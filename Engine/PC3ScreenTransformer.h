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
	Vec3& Transform(Vec3& v) const
	{
		const float zInvert = 1.0f / v.z;
		v.x = (v.x * zInvert + 1.0f) * xFactor;
		v.y = (-v.y * zInvert + 1.0f) * yFactor;
		return v;
	}
	Vec3 GetTransformed(const Vec3& v) const
	{
		return Transform(Vec3(v));
	}
private:
	float xFactor;
	float yFactor;
};