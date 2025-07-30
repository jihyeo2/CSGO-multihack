#pragma once

class Vector4
{
public:
	float x, y, z, w;

	Vector4()
	{
		x = y = z = w = 0;
	}

	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

using Vec4 = Vector4;
using vec4 = Vector4;
using vector4 = Vector4;