#include "UI/Event.hpp"

Event::Event(const std::string& name, EventVariant&& data)
	: m_name(name), m_data(data)
{
}

const std::string& Event::getName() const
{
	return m_name;
}
