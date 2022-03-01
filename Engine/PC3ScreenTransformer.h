#pragma once

#include "Graphics.h"
#include "Vec3.h"

class PC3ScreenTransformer
{
public:
	PC3ScreenTransformer()
		:
		xFactor(float(Graphics::ScreenWidth) / 2.0f),
		yFactor(float(Graphics::ScreenHeight) / 2.0f)
	{}
	template<class Vertex>
	Vertex& Transform(Vertex& v) const
	{
		const float zInv = 1.0f / v.pos.z;
		v *= zInv;
		v.pos.x = (v.pos.x + 1.0f) * xFactor;
		v.pos.y = (-v.pos.y + 1.0f) * yFactor;
		v.pos.z = zInv;

		return v;
	}
	template<class Vertex>
	Vertex GetTransformed(const Vertex& v) const
	{
		return Transform(Vertex(v));
	}
private:
	float xFactor;
	float yFactor;
};