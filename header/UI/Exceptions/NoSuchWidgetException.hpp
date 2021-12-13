#pragma once

#include <exception>
#include <sstream>
#include <string>

class NoSuchWidgetException : public std::exception {
public:
	NoSuchWidgetException(const std::string& name, const std::string& parentName) noexcept
	{
		wholeMessage += "Can't find widget '" + name + "' in '" + parentName + "'";
	}

	NoSuchWidgetException(void* ptr, const std::string& parentName) noexcept
	{
		std::stringstream ss;
		ss << ptr;

		std::string name = ss.str();

		wholeMessage += "Can't find widget '" + name + "' in '" + parentName + "'";
	}

	const char* what() const noexcept {
		return wholeMessage.c_str();
	}

private:
	std::string wholeMessage;
};

