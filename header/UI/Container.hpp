#pragma once

#include "UI/Widget.hpp"

class Container : public Widget
{
public:
	void add(Widget::Ptr widget, const std::string& name);
	void remove(const std::string& name);

	Widget::Ptr get(const std::string& name);
	Widget::ConstPtr get(const std::string& name) const;

protected:
	std::unordered_map<std::string, Widget::Ptr> m_subwidgets;
};
