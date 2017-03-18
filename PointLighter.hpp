#pragma once
#include <vector>
#include "primitives.hpp"
#include "Depth.hpp"

namespace PointLighter
{
	void calculateAmbientLight(std::vector<Point>& points, const Color& ambientColor);
	void calcuateDepthShading(std::vector<Point>& points, const Depth& depth);
}