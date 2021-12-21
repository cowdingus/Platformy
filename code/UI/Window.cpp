#include "UI/Window.hpp"

#include "UI/Button.hpp"
#include "UI/Defaults.hpp"
#include "UI/GUIRoot.hpp"
#include <iostream>

Window::Window()
{
	m_childWidgetsOffset = {0.f, 32.f};

	m_closeButton = Button::create();

	m_closeButton->setProperties(
			Button::Properties {
				.bgFillColor = sf::Color::White,
				.bgOutlineColor = sf::Color::Black,
				.bgOutlineThickness = -1.f,
				.automaticSizing = true,
				.txtSize = 14.f,
				.txtFillColor = sf::Color::Black,
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
	auto closeButtonSize = sf::Vector2f{m_closeButton->getLocalBounds().width, m_closeButton->getLocalBounds().height};
	auto padding = sf::Vector2f{8.f, 5.5f};

	m_titleBarShape.setSize({mySize.x, 32.f});
	m_titleBarShape.setFillColor(sf::Color::White);
	m_titleBarShape.setOutlineThickness(-1);
	m_titleBarShape.setOutlineColor(sf::Color::Black);

	m_closeButton->setPosition({mySize.x - padding.x - closeButtonSize.x - m_childWidgetsOffset.x, padding.y - m_childWidgetsOffset.y});
}

void Window::setSize(sf::Vector2f size)
{
	m_shape.setSize(size);
	m_size = size;
	
	calculateAppearance();
}

void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(m_position);

	target.draw(m_shape, states);
	target.draw(m_titleBarShape, states);

	// Time to draw the subwidgets, lets apply the child widget offsets
	states.transform.translate(m_childWidgetsOffset);

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
	// Avoided the use of sf::Transform because it also computes the rotation and scale, which is unneeded
	return {m_position.x, m_position.y, m_shape.getGlobalBounds().width, m_shape.getGlobalBounds().height};
}

sf::FloatRect Window::getLocalBounds() const
{
	return m_shape.getLocalBounds();
}

Window::Ptr Window::create()
{
	return std::make_shared<Window>();
}

bool Window::isOnTitleBar(sf::Vector2f pos) const
{
	return 32 > pos.y && pos.y > 0;
}

void Window::onMousePress(sf::Vector2f pos, sf::Mouse::Button btn)
{
	if (btn == sf::Mouse::Left)
	{
		std::cout << pos.x << ", " << pos.y << std::endl;
		m_titleBarOnDrag = isOnTitleBar(pos);
	}
}

void Window::onMouseMove(sf::Vector2f pos, sf::Vector2f deltaPos)
{
	if (m_titleBarOnDrag)
	{
		m_position += deltaPos;
	}
}

void Window::onMouseRelease(sf::Vector2f pos, sf::Mouse::Button btn)
{
	if (btn == sf::Mouse::Left && m_titleBarOnDrag)
	{
		m_titleBarOnDrag = false;
	}
}
