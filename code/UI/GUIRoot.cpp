#include "UI/GUIRoot.hpp"

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
		case sf::Event::MouseButtonPressed: {
			sf::Vector2f mousePosition = m_window->mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
			auto pressedWidget = getWidgetBelowPosition(mousePosition);
			std::cout << "Widget pressed" << pressedWidget << std::endl;
			return true;
			break;
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
		if (widget->getGlobalBounds().contains(position))
		{
			return widget;
		}
	}

	return {nullptr};
}
