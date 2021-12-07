#pragma once

#include "Widget.hpp"

#include <unordered_map>
#include <string>

class GUIRoot : public sf::Drawable
{
public:
	GUIRoot(sf::RenderTarget& target);

	void add(Widget::Ptr widget, const std::string& name);
	void remove(const std::string& name);

	Widget::Ptr get(const std::string& name);
	Widget::ConstPtr get(const std::string& name) const;

	bool handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	Widget::Ptr getWidgetBelowPosition(sf::Vector2f mousePosition);

private:
	std::unordered_map<std::string, Widget::Ptr> m_widgets;

	sf::RenderTarget* m_window {nullptr};

	Widget::Ptr m_depressedWidget;
};
