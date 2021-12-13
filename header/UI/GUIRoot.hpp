#pragma once

#include "Widget.hpp"

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
	Widget::Ptr getWidgetBelowPosition(sf::Vector2f mousePosition);

	bool onMousePress(const sf::Event::MouseButtonEvent& e);
	bool onMouseRelease(const sf::Event::MouseButtonEvent& e);
	bool onTextEnter(const sf::Event::TextEvent& e);

	void onClick(sf::Vector2f pos, sf::Mouse::Button btn, Widget::Ptr widget);
	void onFocusChange(Widget::Ptr newFocus);

private:
	std::vector<Widget::Ptr> m_widgets;

	sf::RenderTarget* m_window {nullptr};

	Widget::Ptr m_currentlyPressedWidget {nullptr};
	Widget::Ptr m_currentlyFocusedWidget {nullptr};
};
