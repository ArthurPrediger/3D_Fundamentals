#pragma once

#include "Graphics.h"
#include "Vec3.h"

class NDCScreenTransformer
{
public:
	NDCScreenTransformer()
		:
		xFactor(float(Graphics::ScreenWidth) / 2.0f),
		yFactor(float(Graphics::ScreenHeight) / 2.0f)
	{}
	template<class Vertex>
	Vertex& Transform(Vertex& v) const
	{
		const float wInv = 1.0f / v.pos.w;
		v *= wInv;

		v.pos.x = (v.pos.x + 1.0f) * xFactor;
		v.pos.y = (-v.pos.y + 1.0f) * yFactor;
		v.pos.w = wInv;

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