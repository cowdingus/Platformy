#pragma once

#include "UI/Widget.hpp"

class Button : public Widget
{
public:
	const static auto type = Widget::Type::Button;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {}
};
