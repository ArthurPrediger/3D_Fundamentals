#pragma once

#include "Vec3.h"
#include <vector>

struct IndexedTriangleList
{
	std::vector<Vec3> vertices;
	std::vector<size_t> indices;
};