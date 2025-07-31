#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <any>
#include <vector>
#include "../../../class/PointEngine/include/PointEngine.h"

using namespace std;
using namespace sf;

typedef Vector2f vec2;

struct OnUseCardContext
{
	OnUseCardContext(PointEngine& pe, vector<any> args, RenderWindow& window, vec2 mousepos)
	:pe(pe), args(args), window(window), mousepos(mousepos) {};
	PointEngine& pe;
	vector<any> args;
	RenderWindow& window;
	vec2 mousepos;
};
