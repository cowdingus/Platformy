#include "Engine/GUI/GUIRoot.hpp"

#include "Engine/GUI/Container.hpp"
#include "Engine/GUI/Event.hpp"
#include "Engine/GUI/Exceptions/NoSuchWidgetException.hpp"

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
		case sf::Event::KeyPressed:
			return onKeyPress(event.key);
		case sf::Event::KeyReleased:
			return onKeyRelease(event.key);
		default: break;
	}

	return false;
}

void GUIRoot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_container, states);
}

bool GUIRoot::onMousePress(const sf::Event::MouseButtonEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	auto currentlyPressedWidget = m_container.getWidgetBelowPosition(pos);

	return m_container.propagateMousePress(m_container.toLocal(pos), e.button);
}

bool GUIRoot::onMouseRelease(const sf::Event::MouseButtonEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	auto theWidgetBelow = m_container.getWidgetBelowPosition(pos);

	return m_container.propagateMouseRelease(m_container.toLocal(pos), e.button);
}

bool GUIRoot::onMouseMove(const sf::Event::MouseMoveEvent& e)
{
	auto pos = m_window->mapPixelToCoords({e.x, e.y});

	m_container.propagateMouseMove(m_container.toLocal(pos), pos - m_previousMousePosition);

	m_previousMousePosition = pos;

	return false;
}

bool GUIRoot::onTextEnter(const sf::Event::TextEvent& e)
{
	return m_container.propagateTextEnter(e.unicode);
}

bool GUIRoot::onKeyPress(const sf::Event::KeyEvent& e)
{

}

bool GUIRoot::onKeyRelease(const sf::Event::KeyEvent& e)
{

}
