#include "UI/Container.hpp"

#include "UI/GUIRoot.hpp"
#include "UI/Exceptions/NoSuchWidgetException.hpp"
#include <memory>

void Container::add(Widget::Ptr widget, const std::string& name)
{
	m_subwidgets.push_back(widget);
	widget->setParent(this);
	widget->setRoot(m_root);
	widget->setName(name);
}

void Container::remove(Widget::Ptr widget)
{
	auto i = std::begin(m_subwidgets);

	while (i != std::end(m_subwidgets))
	{
		if ((*i) == widget)
		{
			(*i)->setParent(nullptr);
			(*i)->setRoot(nullptr);
			(*i)->onDetachFromParent();
			(*i)->onDetachFromRoot();
			m_subwidgets.erase(i);
			return;
		}

		if ((*i)->isContainer())
		{
			auto container = std::static_pointer_cast<Container>(*i);
			container->remove(widget);
		}

		++i;
	}

	throw NoSuchWidgetException(widget.get(), getName());
}

void Container::remove(const std::string& name)
{
	try
	{
		remove(get(name));
	}
	catch(NoSuchWidgetException& e)
	{
		throw NoSuchWidgetException(name, getName());
	}
}

Widget::Ptr Container::get(const std::string& name)
{
	// Search for immediate child with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->getName() == name)
		{
			return widget;
		}
	}

	// Recursively search for subchildren with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->isContainer())
		{
			auto container = std::static_pointer_cast<Container>(widget);
			auto subwidget = container->get(name);

			if (subwidget) return subwidget;
		}
	}

	return {nullptr};
}

Widget::ConstPtr Container::get(const std::string& name) const
{
	// Search for immediate child with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->getName() == name)
		{
			return widget;
		}
	}

	// Recursively search for subchildren with that name
	for (const auto& widget : m_subwidgets)
	{
		if (widget->isContainer())
		{
			auto container = std::static_pointer_cast<Container>(widget);
			auto subwidget = container->get(name);

			if (subwidget) return subwidget;
		}
	}

	return {nullptr};
}

void Container::setRoot(GUIRoot* root)
{
	m_root = root;

	for (const auto& widget : m_subwidgets)
	{
		widget->setRoot(root);
	}
}

Widget::Ptr Container::getWidgetBelowPosition(sf::Vector2f pos) const
{
	pos = getInverseTransform().transformPoint(pos);

	for (const auto& widget : m_subwidgets)
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
