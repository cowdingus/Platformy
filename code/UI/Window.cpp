#include "UI/Window.hpp"

Window::Window()
{
	// Default initialize m_props
	setProperties(Properties{});
}

void Window::setProperties(const Window::Properties& props)
{
	m_props = props;
	m_shape.setFillColor(m_props.fillColor);
	m_shape.setOutlineColor(m_props.outlineColor);
	m_shape.setOutlineThickness(m_props.outlineThickness);
	setSize(m_props.size);
}

Window::Properties Window::getProperties() const { return m_props; }

void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_shape, states);

	for (const auto& [name, widget] : m_subwidgets)
	{
		target.draw(*widget, states);
	}
}

bool Window::isOnTopOfWidget(sf::Vector2f pos) const
{
	return getGlobalBounds().contains(pos);
}

sf::FloatRect Window::getGlobalBounds() const
{
	return getTransform().transformRect(m_shape.getGlobalBounds());
}

sf::FloatRect Window::getLocalBounds() const
{
	return m_shape.getLocalBounds();
}

Window::Ptr Window::create()
{
	return std::make_shared<Window>();
}
