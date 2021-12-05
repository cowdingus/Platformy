#pragma once

#include "Widget.hpp"

#include <unordered_map>
#include <string>

class GUIRoot : public sf::Drawable
{
public:
	void add(Widget::Ptr widget, const std::string& name);
	void remove(const std::string& name);

	Widget::Ptr get(const std::string& name);
	Widget::ConstPtr get(const std::string& name) const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::unordered_map<std::string, Widget::Ptr> m_widgets;
};

inline void GUIRoot::add(Widget::Ptr widget, const std::string& name)
{
	m_widgets.emplace(std::make_pair(name, widget));
}

inline void GUIRoot::remove(const std::string& name)
{
	m_widgets.erase(name);
}

inline Widget::Ptr GUIRoot::get(const std::string& name)
{
	return m_widgets.at(name);
}

inline Widget::ConstPtr GUIRoot::get(const std::string& name) const
{
	return m_widgets.at(name);
}

inline void GUIRoot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& [name, widget] : m_widgets)
	{
		target.draw(*widget);
	}
}
