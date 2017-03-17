#pragma once
#include <vector>
#include <primitives.hpp>

namespace PointLighter
{
	void calculateLight(std::vector<Point>& points, const Color& ambientColor);
}