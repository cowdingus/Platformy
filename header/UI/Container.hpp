#pragma once

#include "UI/Widget.hpp"

class Container : public Widget
{
public:
	typedef std::shared_ptr<Container> Ptr;
	typedef std::shared_ptr<const Container> ConstPtr;

	void add(Widget::Ptr widget, const std::string& name);
	void remove(Widget::Ptr widget);
	void remove(const std::string& name);

	Widget::Ptr get(const std::string& name);
	Widget::ConstPtr get(const std::string& name) const;

	bool isContainer() const override;
	void setRoot(class GUIRoot* root) override;

	Widget::Ptr getWidgetBelowPosition(sf::Vector2f position) const;
	Widget::Ptr getImmediateWidgetBelowPosition(sf::Vector2f position) const;

	sf::Vector2f getChildWidgetsOffset() const { return m_childWidgetsOffset; }

	bool propagateTextEnter(uint32_t unicode);

	void propagateOnFocus();
	void propagateOnOutOfFocus();

	void propagateMouseMove(sf::Vector2f pos, sf::Vector2f delta);
	bool propagateMouseRelease(sf::Vector2f pos, sf::Mouse::Button btn);
	bool propagateMousePress(sf::Vector2f pos, sf::Mouse::Button btn);

	const std::vector<Widget::Ptr>& getChildWidgets();

protected:
	bool shouldChangeFocus(Widget::Ptr widget);
	void changeFocusTo(Widget::Ptr widget);

	bool shouldFireClickEvent(Widget::Ptr widget);

	std::weak_ptr<Widget> m_currentlyFocusedWidget;
	std::weak_ptr<Widget> m_previouslyPressedWidget;

	std::vector<Widget::Ptr> m_subwidgets;
	sf::Vector2f m_childWidgetsOffset;

	// Calculates the mouse position in the subwidget space coordinate system
	// (it calculates the mouse position in consideration of m_childWidgetsOffset)
	sf::Vector2f toSubwidgetSpace(sf::Vector2f position) const;
};

inline bool Container::isContainer() const
{
	return true;
}
