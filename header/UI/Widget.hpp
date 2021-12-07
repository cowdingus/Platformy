#pragma once

#include <SFML/Graphics.hpp>

#include <SFML/Graphics/Transformable.hpp>
#include <type_traits>
#include <cassert>
#include <memory>

class Widget : public sf::Drawable, public sf::Transformable, public std::enable_shared_from_this<Widget>
{
public:
	typedef std::shared_ptr<Widget> Ptr;
	typedef std::shared_ptr<const Widget> ConstPtr;

	virtual void setSize(sf::Vector2f size) { m_size = size; }
	sf::Vector2f getSize() const { return m_size; }

	virtual sf::FloatRect getGlobalBounds() const = 0;
	virtual sf::FloatRect getLocalBounds() const = 0;

	template <typename T>
	std::shared_ptr<T> cast();

	template <typename T>
	std::shared_ptr<const T> cast() const;

	virtual bool isOnTopOfWidget(sf::Vector2f point) const = 0;

	virtual void onMouseClick(sf::Mouse::Button mb) {};
	virtual void onMousePress(sf::Mouse::Button btn, sf::Vector2f pos) {};
	virtual void onMouseRelease(sf::Mouse::Button btn, sf::Vector2f position) {};
	virtual void onMouseEnter() {};
	virtual void onMouseLeave() {};

protected:
	sf::Vector2f m_size;
};

template <typename T>
inline std::shared_ptr<T> Widget::cast()
{
	return std::dynamic_pointer_cast<T>(shared_from_this());
}

template <typename T>
inline std::shared_ptr<const T> Widget::cast() const
{
	return std::dynamic_pointer_cast<const T>(shared_from_this());
}
