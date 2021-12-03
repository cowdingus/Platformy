#pragma once

#include <SFML/Graphics.hpp>

#include <type_traits>
#include <cassert>

class Widget : public sf::Drawable, public sf::Transformable
{
public:
	enum class Type
	{
		Window,
		Button,
		Widget
	};

	virtual constexpr Type getType() const = 0;

	virtual void setSize(sf::Vector2f size) { m_size = size; }
	sf::Vector2f getSize() const { return m_size; }

	virtual sf::FloatRect getGlobalBounds() const
	{
		return getTransform().transformRect(getShape().getGlobalBounds());
	}

	virtual sf::FloatRect getLocalBounds() const
	{
		return getShape().getLocalBounds();
	}

	template <typename T>
	T& cast();

	template <typename T>
	const T& cast() const;

	std::function<void()> onMouseClick {};
	std::function<void()> onMouseHover {};
	std::function<void()> onDepressed {};

protected:
	sf::Vector2f m_size;

	virtual const sf::Shape& getShape() const;
};

template <typename T>
inline T& Widget::cast()
{
	static_assert(
			std::is_base_of<Widget, T>::value,
			"Widget casting failed: Tried to cast to a non-widget class"
	);

	assert(
			this->getType() == T::type &&
			"Widget casting failed: Tried to cast to unmatching class"
	);

	return *(static_cast<T*>(this));
}

template <typename T>
inline const T& Widget::cast() const
{
	static_assert(
			std::is_base_of<Widget, T>::value,
			"Widget casting failed: Tried to cast to a non-widget class"
	);

	assert(
			this->getType() == T::type &&
			"Widget casting failed: Tried to cast to unmatching class"
	);

	return *(static_cast<T*>(this));
}
