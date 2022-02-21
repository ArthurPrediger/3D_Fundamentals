#pragma once

#include "Vec3.h"
#include <vector>

template <class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList(std::vector<T> verts_in, std::vector<size_t> indices_in)
		:
		vertices(verts_in),
		indices(indices_in)
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}
	std::vector<T> vertices;
	std::vector<size_t> indices;
};