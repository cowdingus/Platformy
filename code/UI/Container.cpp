#include "UI/Container.hpp"
#include "UI/GUIRoot.hpp"

void Container::add(Widget::Ptr widget, const std::string& name)
{
	m_subwidgets.emplace(std::make_pair(name, widget));
	widget->setParent(this);
	widget->setName(name);
	widget->setRoot(m_root);
}

void Container::remove(const std::string& name)
{
	m_subwidgets.at(name)->setParent(nullptr);
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

void Container::setRoot(GUIRoot* root)
{
	m_root = root;

	for (const auto& [name, widget] : m_subwidgets)
	{
		widget->setRoot(root);
	}
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
