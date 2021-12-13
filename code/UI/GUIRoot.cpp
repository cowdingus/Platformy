#include "UI/GUIRoot.hpp"

#include "UI/Container.hpp"
#include "UI/Exceptions/NoSuchWidgetException.hpp"

#include <iostream>

GUIRoot::GUIRoot(sf::RenderTarget& target)
	: m_window(&target)
{

}

void GUIRoot::add(Widget::Ptr widget, const std::string& name)
{
	m_widgets.push_back(widget);
	widget->setName(name);
	widget->setRoot(this);
	widget->onAttachToRoot();
}

void GUIRoot::remove(Widget::Ptr widget)
{
	auto i = std::begin(m_widgets);

	while (i != std::end(m_widgets))
	{
		if ((*i) == widget)
		{
			(*i)->setRoot(nullptr);
			(*i)->onDetachFromRoot();
			m_widgets.erase(i);
			return;
		}

		if ((*i)->isContainer())
		{
			auto container = std::static_pointer_cast<Container>(*i);
			container->remove(widget);
		}

		++i;
	}

	throw NoSuchWidgetException(widget.get(), "_gui_root");
}

void GUIRoot::remove(const std::string& name)
{
	try
	{
		remove(get(name));
	}
	catch(NoSuchWidgetException& e)
	{
		throw NoSuchWidgetException(name, "_gui_root");
	}
}

Widget::Ptr GUIRoot::get(const std::string& name)
{
	// Search for immediate child with that name
	for (const auto& widget : m_widgets)
	{
		if (widget->getName() == name)
		{
			return widget;
		}
	}

	// Recursively search for subchildren with that name
	for (const auto& widget : m_widgets)
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

Widget::ConstPtr GUIRoot::get(const std::string& name) const
{
	// Search for immediate child with that name
	for (const auto& widget : m_widgets)
	{
		if (widget->getName() == name)
		{
			return widget;
		}
	}

	// Recursively search for subchildren with that name
	for (const auto& widget : m_widgets)
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

bool GUIRoot::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
			return onMousePress(event.mouseButton);
		case sf::Event::MouseButtonReleased:
			return onMouseRelease(event.mouseButton);
		case sf::Event::TextEntered:
			return onTextEnter(event.text);
		default: break;
	}

	return false;
}

void GUIRoot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& widget : m_widgets)
	{
		target.draw(*widget);
	}
}

Widget::Ptr GUIRoot::getWidgetBelowPosition(sf::Vector2f position)
{
	for (const auto& widget : m_widgets)
	{
		// Skip if not on top
		if (!widget->isOnTopOfWidget(position)) continue;

		// Immediately return if widget doesn't have subwidgets
		if (!widget->isContainer()) return widget;

		// Recursively get to the bottom of the container
		auto container = std::static_pointer_cast<Container>(widget);
		auto subwidget = container->getWidgetBelowPosition(position);

		return subwidget ? subwidget : widget;
	}

	return {nullptr};
}

bool GUIRoot::onMousePress(const sf::Event::MouseButtonEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	m_currentlyPressedWidget = getWidgetBelowPosition(pos);

	std::cout << "[GUIRoot::onMousePress]: Press, onWidget: " << m_currentlyPressedWidget << std::endl;

	if (m_currentlyPressedWidget != m_currentlyFocusedWidget) onFocusChange(m_currentlyPressedWidget);

	return static_cast<bool>(m_currentlyPressedWidget);
}

bool GUIRoot::onMouseRelease(const sf::Event::MouseButtonEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	// Checks if mouse button is released on the same widget as the previously pressed one
	auto onWidget = getWidgetBelowPosition(pos);
	if (m_currentlyPressedWidget && m_currentlyPressedWidget == onWidget)
	{
		onClick(pos, e.button, m_currentlyPressedWidget);
	}

	m_currentlyPressedWidget = {nullptr};

	std::cout << "[GUIRoot::onMouseRelease]: Release, onWidget: " << onWidget << std::endl;

	return static_cast<bool>(onWidget);
}

bool GUIRoot::onTextEnter(const sf::Event::TextEvent& e)
{
	if (m_currentlyFocusedWidget)
	{
		std::cout << "[GUIRoot::onTextEnter]: Enter, onWidget: " << m_currentlyFocusedWidget
			<< " unicode: " << e.unicode << std::endl;
		m_currentlyFocusedWidget->onTextEnter(e.unicode);
		return true;
	}

	return false;
}

void GUIRoot::onClick(sf::Vector2f pos, sf::Mouse::Button btn, Widget::Ptr widget)
{
	widget->onClick(pos, btn);
}

void GUIRoot::onFocusChange(Widget::Ptr widgetToFocus)
{
	// TODO(zndf): Fix the edge case when currently focused widget became unfocusable

	// Clear focus if user pressed the void or a unfocusable widget
	if (!widgetToFocus || !widgetToFocus->focusable())
	{
		if (m_currentlyFocusedWidget) m_currentlyFocusedWidget->onOutOfFocus();
		m_currentlyFocusedWidget = nullptr;
		std::cout << "[GUIRoot::onFocusChange]: Unfocused" << std::endl;
	}

	// Change focus if pressed widget is not the same widget as the currently focused one
	// and it is focusable
	if (m_currentlyFocusedWidget != widgetToFocus && widgetToFocus->focusable())
	{
		auto m_previouslyFocusedWidget = m_currentlyFocusedWidget;

		if (m_currentlyFocusedWidget) m_currentlyFocusedWidget->onOutOfFocus();
		m_currentlyFocusedWidget = widgetToFocus;
		m_currentlyFocusedWidget->onFocus();

		std::cout << "[GUIRoot::onFocusChange]: Changed focus, from: "
			<< m_previouslyFocusedWidget << " to: " << m_currentlyFocusedWidget << std::endl;
	}
}
