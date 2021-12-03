#pragma once

#include <SFML/Graphics.hpp>

class World;

class BlockPalette : public sf::Drawable, public sf::Transformable
{
public:
	BlockPalette(const World& world);

	void onMousePressed(const sf::Event::MouseButtonEvent& e);
	void onMouseReleased(const sf::Event::MouseButtonEvent& e);
	void onMouseMove(const sf::Event::MouseMoveEvent& e);

	int getSelectedBlock() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	const World* m_world {nullptr};
	int m_selectedBlock = -1;

	sf::RectangleShape m_windowShape;
};
