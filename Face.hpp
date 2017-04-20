#pragma once
#include "CommonTypeAliases.hpp"
#include "primitives.hpp"
#include "Vertex.hpp"
#include <vector>

struct Face
{
	// Cannot default construct
	// consider using pointers
	std::vector<Vertex*> vertices;
	Point4D normal;
	std::vector<int> vertexIndices;
	Point4D getFaceNormal() const;
};