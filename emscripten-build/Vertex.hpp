#pragma once
#include "CommonTypeAliases.hpp"
#include "primitives.hpp"
#include <vector>
#include <experimental/optional>

struct Vertex
{
	Point4D location;
	std::vector<Point4D> faceNormals;
	std::experimental::optional<Point> assignedNormal;
};