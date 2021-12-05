#pragma once

#include "UI/Widget.hpp"
#include <unordered_map>

class Window : public Widget
{
public:
	typedef std::shared_ptr<Window> Ptr;
	typedef std::shared_ptr<const Window> ConstPtr;

	Window();

	static Window::Ptr create();

	struct Properties
	{
		sf::Color fillColor {6, 26, 66, 235};
		sf::Color outlineColor {sf::Color::Red};
		float outlineThickness {0};
		sf::Vector2f size {200, 160};
	};

	void setSize(sf::Vector2f size) override
	{
		m_shape.setSize(size);
		m_size = size;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setProperties(const Properties& props);
	Properties getProperties() const;

	void add(Widget::Ptr widget, const std::string& name);
	void remove(const std::string& name);

protected:
	const sf::Shape& getShape() const override { return m_shape; }

private:
	sf::RectangleShape m_shape;
	Properties m_props;

	std::unordered_map<std::string, Widget::Ptr> m_subwidgets;
};
