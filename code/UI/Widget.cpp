#include "UI/Widget.hpp"

#include <sstream>
#include <stdexcept>

size_t Widget::addEventListener(const std::string& eventName, const EventListener& callback)
{
	// Create eventListenerMap of `eventName` if not exist
	if (!m_eventListenerMaps.contains(eventName))
	{
		m_eventListenerMaps.emplace(std::make_pair(eventName, EventListenerMap{}));
	}

	// Add an event listener to eventListenerMap of `eventName`
	auto& eventListenerMap = m_eventListenerMaps.at(eventName);
	eventListenerMap.emplace(std::make_pair(++m_lastEventListenerIndex, callback));

	return m_lastEventListenerIndex;
}

void Widget::removeEventListener(const std::string& eventName, EventListenerHandle handle)
{
	// Remove EventListener of `handle` if exists
	if (auto& eventListenerMap = m_eventListenerMaps.at(eventName); eventListenerMap.contains(handle))
	{
		eventListenerMap.erase(handle);
	}
	else
	{
		std::stringstream ss;
		ss << "There's no EventListener of " << handle
			<< " in EventListenerMap of " << eventName << "to remove";

		throw std::runtime_error(ss.str());
	}
}

Widget::EventListener& Widget::getEventListener(const std::string& eventName, EventListenerHandle handle)
{
	return m_eventListenerMaps.at(eventName).at(handle);
}

void Widget::onEvent(Event event)
{
	if (m_eventListenerMaps.contains(event.getName()))
	{
		auto& eventListenerMap = m_eventListenerMaps.at(event.getName());

		for (const auto& [handle, eventListener] : eventListenerMap)
		{
			eventListener(event);
		}
	}
}
