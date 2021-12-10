#include "UI/Button.hpp"

Button::Button()
{
	// Default initialize m_props
	setProperties(Properties{});
}

void Button::setProperties(const Button::Properties& props)
{
	m_props = props;

	m_shape.setFillColor(m_props.bgFillColor);
	m_shape.setOutlineColor(m_props.bgOutlineColor);
	m_shape.setOutlineThickness(m_props.bgOutlineThickness);

	m_text.setFillColor(m_props.txtFillColor);
	m_text.setString(m_props.string);
	m_text.setFont(*m_props.font);
	m_text.setCharacterSize(m_props.txtSize);

	setSize(m_props.size);

	calculateAppearance();
}

Button::Properties Button::getProperties() const { return m_props; }

void Button::calculateAppearance()
{
	if (m_props.automaticSizing)
	{
		sf::Vector2f padding = {12.f, 8.f};

		sf::FloatRect textRect = m_text.getLocalBounds();
		setSize({textRect.left + textRect.width + padding.x, textRect.top + textRect.height + padding.y});
	}

	// Position the text into the center of the button
	sf::Vector2f backgroundSize = m_shape.getSize();
	sf::FloatRect textRect = m_text.getLocalBounds();

	m_text.setOrigin(
			textRect.left + textRect.width/2.0f,
			textRect.top  + textRect.height/2.0f);

	m_text.setPosition({backgroundSize.x/2.0f, backgroundSize.y/2.0f});
}

bool Button::isOnTopOfWidget(sf::Vector2f point) const
{
	return getGlobalBounds().contains(point);
}

sf::FloatRect Button::getGlobalBounds() const
{
	return getTransform().transformRect(m_shape.getGlobalBounds());
}

sf::FloatRect Button::getLocalBounds() const
{
	return m_shape.getLocalBounds();
}

void Button::setOnClick(const std::function<void ()>& callback)
{
	m_onClickCallback = callback;
}

void Button::onClick(sf::Vector2f pos, sf::Mouse::Button btn)
{
	if (btn == sf::Mouse::Left && m_onClickCallback) m_onClickCallback();
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_shape, states);
	target.draw(m_text, states);
}
