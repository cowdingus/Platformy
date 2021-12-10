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

Widget::Ptr Container::getWidgetBelowPosition(sf::Vector2f pos) const
{
	pos = getInverseTransform().transformPoint(pos);

	for (const auto& [name, widget] : m_subwidgets)
	{
		if (!widget->isOnTopOfWidget(pos)) continue;
		if (!widget->isContainer()) return widget;

		// Recursively get to the bottom of the container
		auto container = widget->cast<Container>();
		auto subwidget = container->getWidgetBelowPosition(pos);

		return subwidget ? subwidget : widget;
	}

	return {nullptr};
}
