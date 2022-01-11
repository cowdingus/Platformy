#include "Engine/GUI/Container.hpp"

#include "Engine/GUI/GUIRoot.hpp"
#include "Engine/GUI/Exceptions/NoSuchWidgetException.hpp"

#include <iostream>
#include <spdlog/spdlog.h>

#include <memory>

void Container::add(Widget::Ptr widget, const std::string& name)
{
	widget->setName(name);
	widget->setParent(this);
	widget->setRoot(m_root);
	m_subwidgets.push_back(widget);
	widget->onAttachToRoot();
	widget->onAttachToParent();
}

void Container::remove(Widget::Ptr widget)
{
	auto i = std::begin(m_subwidgets);

	while (i != std::end(m_subwidgets))
	{
		if ((*i) == widget)
		{
			(*i)->setParent(nullptr);
			(*i)->setRoot(nullptr);
			(*i)->onDetachFromParent();
			(*i)->onDetachFromRoot();
			m_subwidgets.erase(i);
			return;
		}

		if ((*i)->isContainer())
		{
			auto container = std::static_pointer_cast<Container>(*i);
			container->remove(widget);
		}

		++i;
	}

	throw NoSuchWidgetException(widget.get(), getName());
}

void Container::remove(const std::string& name)
{
	try
	{
		remove(get(name));
	}
	catch(NoSuchWidgetException& e)
	{
		throw NoSuchWidgetException(name, getName());
	}
}

Widget::Ptr Container::get(const std::string& name)
{
	// Search for immediate child with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->getName() == name)
		{
			return widget;
		}
	}

	// Recursively search for subchildren with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->isContainer())
		{
			auto container = std::static_pointer_cast<Container>(widget);
			auto subwidget = container->get(name);

			if (subwidget) return subwidget;
		}
	}

	return {nullptr};
}

Widget::ConstPtr Container::get(const std::string& name) const
{
	// Search for immediate child with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->getName() == name)
		{
			return widget;
		}
	}

	// Recursively search for subchildren with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->isContainer())
		{
			auto container = std::static_pointer_cast<Container>(widget);
			auto subwidget = container->get(name);

			if (subwidget) return subwidget;
		}
	}

	return {nullptr};
}

void Container::setRoot(GUIRoot* root)
{
	m_root = root;

	for (const auto& widget : m_subwidgets)
	{
		widget->setRoot(root);
	}
}

Widget::Ptr Container::getWidgetBelowPosition(sf::Vector2f pos) const
{
	pos = toSubwidgetSpace(pos);

	for (const auto& widget : m_subwidgets)
	{
		if (!widget->isOnTopOfWidget(pos)) continue;
		if (!widget->isContainer()) return widget;

		// Recursively get to the bottom of the container
		auto container = widget->cast<Container>();
		auto subwidget = container->getWidgetBelowPosition(pos);

		return subwidget ? subwidget : widget;
	}

	return {nullptr};
}

Widget::Ptr Container::getImmediateWidgetBelowPosition(sf::Vector2f pos) const
{
	pos = toSubwidgetSpace(pos);

	for (const auto& widget : m_subwidgets)
	{
		if (widget->isOnTopOfWidget(pos)) return widget;
	}

	return {};
}

sf::Vector2f Container::toSubwidgetSpace(sf::Vector2f position) const
{
	const auto& offset = getChildWidgetsOffset();

	return position - offset;
}

const std::vector<Widget::Ptr>& Container::getChildWidgets()
{
	return m_subwidgets;
}

bool Container::propagateTextEnter(uint32_t unicode)
{
	if (auto focusedWidget = m_currentlyFocusedWidget.lock(); focusedWidget)
	{
		focusedWidget->onTextEnter(unicode);

		if (focusedWidget->isContainer())
		{
			const auto& container = std::static_pointer_cast<Container>(focusedWidget);
			container->propagateTextEnter(unicode);
		}

		return true;
	}

	return false;
}

void Container::propagateOnFocus()
{
	if (auto focusedWidget = m_currentlyFocusedWidget.lock(); focusedWidget)
	{
		focusedWidget->onFocus();

		if (focusedWidget->isContainer())
		{
			const auto& container = std::static_pointer_cast<Container>(focusedWidget);
			container->propagateOnFocus();
		}
	}
}

void Container::propagateOnOutOfFocus()
{
	if (auto focusedWidget = m_currentlyFocusedWidget.lock(); focusedWidget)
	{
		focusedWidget->onOutOfFocus();

		if (focusedWidget->isContainer())
		{
			const auto& container = std::static_pointer_cast<Container>(focusedWidget);
			container->propagateOnOutOfFocus();
		}
	}
}

void Container::propagateMouseMove(sf::Vector2f pos, sf::Vector2f delta)
{
	pos = toSubwidgetSpace(pos);

	for (const auto& subwidget : m_subwidgets)
	{
		subwidget->onMouseMove(subwidget->toLocal(pos), delta);

		if (subwidget->isContainer())
		{
			const auto& container = std::static_pointer_cast<Container>(subwidget);
			container->propagateMouseMove(subwidget->toLocal(pos), delta);
		}
	}
}

bool Container::propagateMousePress(sf::Vector2f pos, sf::Mouse::Button btn)
{
	pos = toSubwidgetSpace(pos);

	for (const auto& subwidget : m_subwidgets)
	{
		if (subwidget->isOnTopOfWidget(pos))
		{
			subwidget->onMousePress(subwidget->toLocal(pos), btn);

			if (shouldChangeFocus(subwidget))
			{
				changeFocusTo(subwidget);
			}

			if (subwidget->isContainer())
			{
				const auto& container = std::static_pointer_cast<Container>(subwidget);
				container->propagateMousePress(subwidget->toLocal(pos), btn);
			}

			m_previouslyPressedWidget = subwidget;
			return true;
		}
	}

	return false;
}

bool Container::propagateMouseRelease(sf::Vector2f pos, sf::Mouse::Button btn)
{
	pos = toSubwidgetSpace(pos);

	for (const auto& subwidget : m_subwidgets)
	{
		if (subwidget->isOnTopOfWidget(pos))
		{
			subwidget->onMouseRelease(subwidget->toLocal(pos), btn);

			if (shouldFireClickEvent(subwidget))
			{
				std::cout << "Firing on click event" << std::endl;
				subwidget->onClick(subwidget->toLocal(pos), btn);
			}

			if (subwidget->isContainer())
			{
				const auto& container = std::static_pointer_cast<Container>(subwidget);
				container->propagateMouseRelease(subwidget->toLocal(pos), btn);
			}

			m_previouslyPressedWidget.reset();
			return true;
		}
	}

	return false;
}

void Container::changeFocusTo(Widget::Ptr widget)
{
	auto currentlyFocusedWidget = m_currentlyFocusedWidget.lock();

	if (currentlyFocusedWidget)
	{
		currentlyFocusedWidget->onOutOfFocus();
	}

	m_currentlyFocusedWidget = widget;

	if (widget)
	{
		widget->onFocus();
	}
}

bool Container::shouldChangeFocus(Widget::Ptr widget)
{
	auto previouslyPressedWidget = m_previouslyPressedWidget.lock();

	return previouslyPressedWidget != widget;
}

bool Container::shouldFireClickEvent(Widget::Ptr widget)
{
	auto previouslyPressedWidget = m_previouslyPressedWidget.lock();

	return previouslyPressedWidget == widget;
}
