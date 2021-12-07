#pragma once

#include "UI/Widget.hpp"
#include <unordered_map>

class Button : public Widget
{
public:
	typedef std::shared_ptr<Button> Ptr;
	typedef std::shared_ptr<const Button> ConstPtr;

	Button();

	static Button::Ptr create() { return std::make_shared<Button>(); }

	struct Properties
	{
		// Background
		sf::Color bgFillColor {sf::Color::Blue};
		sf::Color bgOutlineColor {sf::Color::Red};
		float bgOutlineThickness {0};

		bool automaticSizing {true};
		sf::Vector2f size {90, 50};

		// Text
		float txtSize {18};
		sf::Color txtFillColor {sf::Color::White};
		sf::String string {"A Button"};
		sf::Font* font {nullptr};
	};

	void setSize(sf::Vector2f size) override
	{
		m_size = size;

		// The background reflects the size of the button
		m_shape.setSize(size);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setProperties(const Properties& props);
	Properties getProperties() const;

protected:
	const sf::Shape& getShape() const override { return m_shape; }

	void calculateAppearance();

private:
	sf::RectangleShape m_shape;
	sf::Text m_text;

	Properties m_props;
};
