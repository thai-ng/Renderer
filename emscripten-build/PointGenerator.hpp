#pragma once
#include <vector>
#include "primitives.hpp"
namespace PointGenerator
{
	std::vector<Point4D> generateLinePoints(const Point4D& p1, const Point4D& p2);
	std::vector<Point4D> generatePolygonPoints(const std::vector<Point4D>& points);
	std::vector<Point4D> generateWireframePoints(const std::vector<Point4D>& points);
}