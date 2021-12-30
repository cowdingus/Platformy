#pragma once

#include "Widget.hpp"
#include "UI/Group.hpp"

#include <vector>
#include <string>

class GUIRoot : public sf::Drawable
{
public:
	GUIRoot(sf::RenderTarget& target);

	void add(Widget::Ptr widget, const std::string& name);
	void remove(Widget::Ptr widget);
	void remove(const std::string& name);

	Widget::Ptr get(const std::string& name);
	Widget::ConstPtr get(const std::string& name) const;

	bool handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	// Primitive Events
	// Primitive events are broadcasted to every widgets in the widget tree
	bool onMousePress(const sf::Event::MouseButtonEvent& e);
	bool onMouseRelease(const sf::Event::MouseButtonEvent& e);
	bool onMouseMove(const sf::Event::MouseMoveEvent& e);
	bool onTextEnter(const sf::Event::TextEvent& e);
	bool onKeyPress(const sf::Event::KeyEvent& e);
	bool onKeyRelease(const sf::Event::KeyEvent& e);

private:
	Group m_container;

	sf::RenderTarget* m_window {nullptr};

	sf::Vector2f m_previousMousePosition;
};
