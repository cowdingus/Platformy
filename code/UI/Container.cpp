#include "UI/Container.hpp"

#include "UI/GUIRoot.hpp"
#include "UI/Exceptions/NoSuchWidgetException.hpp"

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

	return position - m_position - offset;
}

const std::vector<Widget::Ptr>& Container::getChildWidgets()
{
	return m_subwidgets;
}

void Container::propagateOnClick(sf::Vector2f pos, sf::Mouse::Button btn)
{
	// Convert the position from this' coordinate space to subwidget space
	// coordinate space (in consideration of getChildWidgetsOffset)
	pos = toSubwidgetSpace(pos);

	if (auto widgetBelow = getImmediateWidgetBelowPosition(pos))
	{
		// Convert the subwidget space coordinate space to the target coordinate
		// space
		pos -= {widgetBelow->getPosition().x, widgetBelow->getPosition().y};

		widgetBelow->onClick(pos, btn);
	}
}

void Container::propagateOnMouseMove(sf::Vector2f pos, sf::Vector2f delta)
{
	// Convert the position from this' coordinate space to subwidget space
	// coordinate space (in consideration of getChildWidgetsOffset)
	pos = toSubwidgetSpace(pos);

	// Tell all the subwidgets about the mouse move change
	for (const auto& subwidget : m_subwidgets)
	{
		// Notice the widget of the mouse move
		pos -= {subwidget->getPosition().x, subwidget->getPosition().y};
		subwidget->onMouseMove(pos, delta);

		// Notice the subwidgets of the widget if it's a container
		if (subwidget->isContainer())
		{
			const auto& container = std::static_pointer_cast<Container>(subwidget);
			container->propagateOnMouseMove(pos, delta);
		}
	}

}

void Container::propagateOnMousePress(sf::Vector2f pos, sf::Mouse::Button btn)
{
	// Convert the position from this' coordinate space to subwidget space
	// coordinate space (in consideration of getChildWidgetsOffset)
	pos = toSubwidgetSpace(pos);

	// Tell the target widget about the mouse press (since mouse press is a
	// targeting event)
	if (auto widgetBelow = getImmediateWidgetBelowPosition(pos))
	{
		// Convert the subwidget space coordinate space to the target coordinate
		// space
		pos -= {widgetBelow->getPosition().x, widgetBelow->getPosition().y};

		widgetBelow->onMousePress(pos, btn);

		// Notice the subwidgets of the widget if it's a container
		if (widgetBelow->isContainer())
		{
			const auto& container = std::static_pointer_cast<Container>(widgetBelow);
			container->propagateOnMousePress(pos, btn);
		}
	}
}

void Container::propagateOnMouseRelease(sf::Vector2f pos, sf::Mouse::Button btn)
{
	// Convert the position from this' coordinate space to subwidget space
	// coordinate space (in consideration of getChildWidgetsOffset)
	pos = toSubwidgetSpace(pos);

	// Tell the target widget about the mouse press (since mouse release is a
	// targeting event)
	if (auto widgetBelow = getImmediateWidgetBelowPosition(pos))
	{
		// Convert the subwidget space coordinate space to the target coordinate
		// space
		pos -= {widgetBelow->getPosition().x, widgetBelow->getPosition().y};

		widgetBelow->onMouseRelease(pos, btn);

		// Notice the subwidgets of the widget if it's a container
		if (widgetBelow->isContainer())
		{
			const auto& container = std::static_pointer_cast<Container>(widgetBelow);
			container->propagateOnMouseRelease(pos, btn);
		}
	}
}
