#include "UI/BlockPalette.hpp"

BlockPalette::BlockPalette(const World& world)
    : m_world(&world)
{
    m_windowShape.setFillColor(sf::Color(45.f, 91.f, 174.f, 184));
    m_windowShape.setSize({200.f, 200.f});
    m_windowShape.setOutlineColor(sf::Color::Black);
    m_windowShape.setOutlineThickness(1.f);
}

void BlockPalette::onMousePressed(const sf::Event::MouseButtonEvent& e)
{

}

void BlockPalette::onMouseReleased(const sf::Event::MouseButtonEvent& e)
{

}

void BlockPalette::onMouseMove(const sf::Event::MouseMoveEvent& e)
{

}


int BlockPalette::getSelectedBlock() const
{
    return m_selectedBlock;
}


void BlockPalette::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_windowShape, states);
}

