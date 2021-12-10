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
}

void GUIRoot::remove(const std::string& name)
{
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
		{
			return onMousePress(event.mouseButton);
		}
		case sf::Event::MouseButtonReleased:
		{
			return onMouseRelease(event.mouseButton);
		}
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
	auto pressedWidget = getWidgetBelowPosition(pos);

	m_currentlyPressedWidget = pressedWidget;

	std::cout << "[GUIRoot::handleEvent]: Press, onWidget: " << pressedWidget << std::endl;

	return static_cast<bool>(pressedWidget);
}

bool GUIRoot::onMouseRelease(const sf::Event::MouseButtonEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	// Checks if mouse button is released on the same widget as the previously pressed one
	auto onWidget = getWidgetBelowPosition(pos);
	if (m_currentlyPressedWidget == onWidget)
	{
		onClick(e, m_currentlyPressedWidget);
	}

	m_currentlyPressedWidget = {nullptr};

	std::cout << "[GUIRoot::handleEvent]: Release, onWidget: " << onWidget << std::endl;

	return static_cast<bool>(onWidget);
}

void GUIRoot::onClick(const sf::Event::MouseButtonEvent& e, Widget::Ptr widget)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});
	widget->onClick(e.button);
}
