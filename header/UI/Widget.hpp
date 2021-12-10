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

	// Returns true if widget inherits Container class, defaults to false
	virtual bool isContainer() const { return false; }

	// Returns true if widget is focusable, defaults to true
	virtual bool focusable() const { return true; }

	template <typename T>
	std::shared_ptr<T> cast();

	template <typename T>
	std::shared_ptr<const T> cast() const;

	virtual bool isOnTopOfWidget(sf::Vector2f point) const = 0;

	virtual void onClick(sf::Vector2f pos, sf::Mouse::Button btn) {};
	virtual void onMousePress(sf::Vector2f pos, sf::Mouse::Button btn) {};
	virtual void onMouseRelease(sf::Vector2f pos, sf::Mouse::Button btn) {};
	virtual void onMouseEnter(sf::Vector2f pos) {};
	virtual void onMouseLeave(sf::Vector2f pos) {};
	virtual void onFocus() {};
	virtual void onOutOfFocus() {};
	virtual void onTextEnter(sf::Uint32 unicode) {};

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
