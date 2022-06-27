#pragma once

#include "Triangle.h"

template <class Vertex>
class DefaultGeometryShader
{
public:
	typedef Vertex Output;
public:
	Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, size_t triangle_index) const
	{
		return { v0, v1, v2 };
	}
};