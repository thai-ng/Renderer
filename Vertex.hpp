#pragma once
#include "CommonTypeAliases.hpp"
#include "primitives.hpp"
#include <vector>
#include <optional>

struct Vertex
{
	Point4D location;
	std::vector<Point4D> faceNormals;
	std::optional<Point> assignedNormal;
};