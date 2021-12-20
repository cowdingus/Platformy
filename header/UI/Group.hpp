#pragma once

#include "UI/Container.hpp"
#include "UI/Widget.hpp"

#include <unordered_map>

class Group : public Container
{
public:
	typedef std::shared_ptr<Group> Ptr;
	typedef std::shared_ptr<const Group> ConstPtr;

	static Group::Ptr create() { return std::make_shared<Group>(); }

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool isOnTopOfWidget(sf::Vector2f pos) const override;

	sf::FloatRect getGlobalBounds() const override;
	sf::FloatRect getLocalBounds() const override;
};

inline void Group::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform.translate(m_position);

	for (const auto& widget : m_subwidgets)
	{
		target.draw(*widget, states);
	}
}

inline bool Group::isOnTopOfWidget(sf::Vector2f pos) const
{
	auto localPos = toSubwidgetSpace(pos);

	for (const auto& widget : m_subwidgets)
	{
		bool isOnTop = widget->isOnTopOfWidget(localPos);

		if (isOnTop) return true;
	}

	return false;
}

inline sf::FloatRect Group::getGlobalBounds() const
{
	return {m_position.x, m_position.y, m_size.x, m_size.y};
}

inline sf::FloatRect Group::getLocalBounds() const
{
	return {0, 0, m_size.x, m_size.y};
}
