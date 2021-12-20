#pragma once

#include "UI/Container.hpp"
#include "UI/Widget.hpp"
#include "UI/Button.hpp"

#include <unordered_map>

class Window : public Container
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

	void setSize(sf::Vector2f size) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setProperties(const Properties& props);
	Properties getProperties() const;

	bool isOnTopOfWidget(sf::Vector2f pos) const override;

	sf::FloatRect getGlobalBounds() const override;
	sf::FloatRect getLocalBounds() const override;

	void onMousePress(sf::Vector2f pos, sf::Mouse::Button btn) override;
	void onMouseRelease(sf::Vector2f pos, sf::Mouse::Button btn) override;
	void onMouseMove(sf::Vector2f pos, sf::Vector2f deltaPos) override;

protected:
	void calculateAppearance();
	bool isOnTitleBar(sf::Vector2f pos) const;

private:
	sf::RectangleShape m_shape;
	sf::RectangleShape m_titleBarShape;
	Button::Ptr m_closeButton {nullptr};

	Properties m_props;

	bool m_titleBarOnDrag = false;
};
