#pragma once

#include "UI/Widget.hpp"

class Window : public Widget
{
public:
	struct Properties
	{
		sf::Color fillColor;
		sf::Color outlineColor;
		float outlineThickness;
		sf::Vector2f size;
	};

	const static auto type = Widget::Type::Window;

	constexpr Widget::Type getType() const override
	{
		return type;
	}

	void setSize(sf::Vector2f size) override
	{
		m_shape.setSize(size);
		m_size = size;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setProperties(const Properties& props);
	Properties getProperties() const;

protected:
	const sf::Shape& getShape() const override { return m_shape; }

private:
	sf::RectangleShape m_shape;
	Properties m_props;

	std::vector<Widget> m_children;
};

inline void Window::setProperties(const Window::Properties& props)
{
	m_props = props;
	m_shape.setFillColor(m_props.fillColor);
	m_shape.setOutlineColor(m_props.outlineColor);
	m_shape.setOutlineThickness(m_props.outlineThickness);
	setSize(m_props.size);
}

inline Window::Properties Window::getProperties() const { return m_props; }

inline void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(m_shape, states);
}
