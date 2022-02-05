#pragma once

#include "Vec3.h"
#include <vector>

struct IndexedTriangleList
{
	IndexedTriangleList(std::vector<Vec3> verts_in, std::vector<size_t> indices_in)
		:
		vertices(verts_in),
		indices(indices_in)
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
		cullFlags.resize(indices.size() / 3, false);
	}
	std::vector<Vec3> vertices;
	std::vector<size_t> indices;
	std::vector<bool> cullFlags;
};