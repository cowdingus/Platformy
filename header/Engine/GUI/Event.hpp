#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <variant>

class Event
{
public:
	struct Click
	{
		sf::Vector2f pos;
		sf::Mouse::Button btn;
	};

	struct MousePress
	{
		sf::Vector2f pos;
		sf::Mouse::Button btn;
	};

	struct MouseRelease
	{
		sf::Vector2f pos;
		sf::Mouse::Button btn;
	};

	struct MouseMove
	{
		sf::Vector2f pos;
		sf::Vector2f delta;
	};

	typedef std::variant<Click, MousePress, MouseRelease, MouseMove> EventVariant;

public:
	Event(const std::string& name, EventVariant&& data);

	const std::string& getName() const;

	template <typename EventData>
	EventData& getData();

	template <typename EventData>
	const EventData& getData() const;

private:
	std::string m_name;

	std::variant<Click, MousePress, MouseRelease, MouseMove> m_data;
};

template <typename EventData>
inline EventData& Event::getData()
{
	return std::get<EventData>(m_data);
}

template <typename EventData>
inline const EventData& Event::getData() const
{
	return std::get<EventData>(m_data);
}
