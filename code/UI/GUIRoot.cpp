#include "UI/GUIRoot.hpp"

#include "UI/Container.hpp"

#include <iostream>

GUIRoot::GUIRoot(sf::RenderTarget& target)
	: m_window(&target)
{

}

void GUIRoot::add(Widget::Ptr widget, const std::string& name)
{
	m_widgets.emplace(std::make_pair(name, widget));
	widget->setRoot(this);
}

void GUIRoot::remove(const std::string& name)
{
	m_widgets.at(name)->setRoot(nullptr);
	m_widgets.erase(name);
}

Widget::Ptr GUIRoot::get(const std::string& name)
{
	return m_widgets.at(name);
}

Widget::ConstPtr GUIRoot::get(const std::string& name) const
{
	return m_widgets.at(name);
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
	for (const auto& [name, widget] : m_widgets)
	{
		target.draw(*widget);
	}
}

Widget::Ptr GUIRoot::getWidgetBelowPosition(sf::Vector2f position)
{
	for (const auto& [name, widget] : m_widgets)
	{
		// Skip if not on top
		if (!widget->isOnTopOfWidget(position)) continue;

		// Immediately return if widget doesn't have subwidgets
		if (!widget->isContainer()) return widget;

		// Recursively get to the bottom of the container
		auto container = widget->cast<Container>();
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
