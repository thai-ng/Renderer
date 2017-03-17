#pragma once
#include <vector>
#include "primitives.hpp"
namespace PointGenerator
{
	std::vector<Point> generateLinePoints(const Point& p1, const Point& p2);
	std::vector<Point> generatePolygonPoints(const std::vector<Point>& points);
	std::vector<Point> generateWireframePoints(const std::vector<Point>& points);
}