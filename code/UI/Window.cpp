#include "UI/Window.hpp"

#include "UI/Button.hpp"
#include "UI/Defaults.hpp"
#include "UI/GUIRoot.hpp"

Window::Window()
{
	m_closeButton = Button::create();

	m_closeButton->setProperties(
			Button::Properties {
				.bgFillColor = sf::Color::White,
				.bgOutlineColor = sf::Color::Black,
				.bgOutlineThickness = 1.f,
				.automaticSizing = true,
				.txtSize = 14.f,
				.string = "x",
				.font = Defaults::getDefaultFont(),
			});

	add(m_closeButton, "_close_btn");

	m_closeButton->setOnClick([this](){if (getRoot()) getRoot()->remove(this->shared_from_this());});

	// Default initialize m_props
	setProperties(Properties{});
}

void Window::setProperties(const Window::Properties& props)
{
	m_props = props;
	m_shape.setFillColor(m_props.fillColor);
	m_shape.setOutlineColor(m_props.outlineColor);
	m_shape.setOutlineThickness(m_props.outlineThickness);

	// Put this at the bottom, it will recalculate the appearance
	setSize(m_props.size);
}

Window::Properties Window::getProperties() const { return m_props; }

void Window::calculateAppearance()
{
	assert(m_closeButton);

	auto mySize = getSize();

	m_closeButton->setPosition({mySize.x - 32, mySize.y - 32});
}

void Window::setSize(sf::Vector2f size)
{
	m_shape.setSize(size);
	m_size = size;
	
	calculateAppearance();
}

void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_shape, states);

	for (const auto& widget : m_subwidgets)
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
