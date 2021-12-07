#include "UI/Container.hpp"

void Container::add(Widget::Ptr widget, const std::string& name)
{
	m_subwidgets.emplace(std::make_pair(name, widget));
}

void Container::remove(const std::string& name)
{
	m_subwidgets.erase(name);
}

Widget::Ptr Container::get(const std::string& name)
{
	return m_subwidgets.at(name);
}

Widget::ConstPtr Container::get(const std::string& name) const
{
	return m_subwidgets.at(name);
}

