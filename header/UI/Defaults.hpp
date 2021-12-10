#pragma once

#include <SFML/Graphics/Font.hpp>

namespace Defaults
{
	inline sf::Font* getDefaultFont()
	{
		static sf::Font defaultFont;
		static bool initialized = false;
		
		if (!initialized)
		{
			if (!defaultFont.loadFromFile("assets/arial.ttf"))
			{
				throw std::runtime_error("Can't load default font: assets/arial.ttf");
			}

			initialized = true;
		}

		return &defaultFont;
	}
}
