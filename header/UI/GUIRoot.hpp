#pragma once

#include "Widget.hpp"

#include <unordered_map>
#include <string>

class GUIRoot
{
public:
	GUIRoot();

	void add(Widget& widget);
	void remove(Widget& widget);

private:
};
