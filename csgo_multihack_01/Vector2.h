#pragma once

#include "pch.h"

class Vector2
{
public:
	float x, y;

	Vector2()
	{
		x = y = 0;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

using Vec2 = Vector2;
using vec2 = Vector2;
using vector2 = Vector2;