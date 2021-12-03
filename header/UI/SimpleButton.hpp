#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

class SimpleButton : public sf::Drawable, public sf::Transformable
{
public:
	SimpleButton();

	void setText(sf::String string);
	void setSize(sf::Vector2f size);
	void setColor(sf::Vector2f color);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::RectangleShape m_buttonShape;
	sf::Text m_text;
};
