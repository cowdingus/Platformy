#include "UI/GUIRoot.hpp"

#include "UI/Container.hpp"
#include "UI/Exceptions/NoSuchWidgetException.hpp"

#include <iostream>
#include <memory>

GUIRoot::GUIRoot(sf::RenderTarget& target)
	: m_window(&target)
{
	m_container.setName("_gui_root");
	m_container.setRoot(this);
}

void GUIRoot::add(Widget::Ptr widget, const std::string& name)
{
	m_container.add(widget, name);
}

void GUIRoot::remove(Widget::Ptr widget)
{
	m_container.remove(widget);
}

void GUIRoot::remove(const std::string& name)
{
	m_container.remove(name);
}

Widget::Ptr GUIRoot::get(const std::string& name)
{
	return m_container.get(name);
}

Widget::ConstPtr GUIRoot::get(const std::string& name) const
{
	return m_container.get(name);
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
		case sf::Event::MouseMoved:
			return onMouseMove(event.mouseMove);
		default: break;
	}

	return false;
}

void GUIRoot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_container, states);
}

Widget::Ptr GUIRoot::getWidgetBelowPosition(sf::Vector2f position)
{
	return m_container.getWidgetBelowPosition(position);
}

bool GUIRoot::onMousePress(const sf::Event::MouseButtonEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	m_currentlyPressedWidget = getWidgetBelowPosition(pos);

	if (!m_currentlyPressedWidget) return false;

	m_currentlyPressedWidget->onMousePress(pos, e.button);

	if (m_currentlyPressedWidget->isContainer())
	{
		std::static_pointer_cast<Container>(m_currentlyPressedWidget)->propagateOnMousePress(pos, e.button);
	}

	std::cout << "[GUIRoot::onMousePress]: Press, onWidget: " << m_currentlyPressedWidget << std::endl;

	if (m_currentlyPressedWidget != m_currentlyFocusedWidget) onFocusChange(m_currentlyPressedWidget);

	return static_cast<bool>(m_currentlyPressedWidget);
}

bool GUIRoot::onMouseRelease(const sf::Event::MouseButtonEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	// Trigger onClick
	auto onWidget = getWidgetBelowPosition(pos);
	if (m_currentlyPressedWidget && m_currentlyPressedWidget == onWidget)
	{
		onClick(pos, e.button, m_currentlyPressedWidget);
	}

	// Propagate onMouseRelease
	m_container.propagateOnMouseRelease(pos, e.button);

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

bool GUIRoot::onMouseMove(const sf::Event::MouseMoveEvent& e)
{
	auto deltaPos = m_window->mapPixelToCoords({e.x, e.y}) - m_currentMousePosition;
	m_currentMousePosition = m_window->mapPixelToCoords({e.x, e.y});

	std::cout << "[GUIRoot::onMouseMove]: Move, newPos: " << e.x << ", " << e.y << std::endl;

	m_container.onMouseMove(m_currentMousePosition, deltaPos);
	m_container.propagateOnMouseMove(m_currentMousePosition, deltaPos);

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
