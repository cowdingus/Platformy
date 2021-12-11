#pragma once

#include <SFML/Graphics.hpp>

#include <type_traits>
#include <cassert>
#include <memory>

class GUIRoot;
class Container;

class Widget : public sf::Drawable, public sf::Transformable, public std::enable_shared_from_this<Widget>
{
public:
	typedef std::shared_ptr<Widget> Ptr;
	typedef std::shared_ptr<const Widget> ConstPtr;

	const std::string& getName() { return m_name; }

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

	// internal
	virtual void onAttachToRoot() {};
	virtual void onDetachFromRoot() {};

	// internal
	virtual void onAttachToParent() {};
	virtual void onDetachFromParent() {};

	// internal
	virtual void setRoot(GUIRoot* root) { m_root = root; }
	GUIRoot* getRoot() const { return m_root; }

	// internal
	void setParent(Container* parent) { m_parent = parent; }
	Container* getParent() const { return m_parent; }

	// internal
	void setName(const std::string& name) { m_name = name; }

protected:
	sf::Vector2f m_size;

	GUIRoot* m_root {nullptr};
	Container* m_parent {nullptr};

	std::string m_name = "_unnamed_widget";

private:
	using sf::Transformable::getRotation;
	using sf::Transformable::setRotation;
	using sf::Transformable::rotate;
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
