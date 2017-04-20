#include "Face.hpp"
#include "LineClipper.h"
#include "primitives.hpp"
#include <algorithm>
Point4D Face::getFaceNormal() const
{/*
	std::vector<Point4D> tempVertices;
	tempVertices.resize(vertices.size());
	std::transform(vertices.begin(), vertices.end(), tempVertices.begin(), [](auto& v) {return v->location; });
	auto sorted = sortVertices(tempVertices);
	std::reverse(sorted.begin(), sorted.end());

	Plane_t face = { sorted[0], sorted[1], sorted[2] };*/
	Plane_t face = { vertices[0]->location, vertices[1]->location, vertices[2]->location };
	auto normal4D = normalize(getNormal(face));
	return normal4D;
}
