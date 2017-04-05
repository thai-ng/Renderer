#pragma once
#include <vector>
#include "primitives.hpp"
#include "Depth.hpp"
#include "Light.hpp"

namespace PointLighter
{
	void calculateAmbientLight(std::vector<Point>& points, const Color& ambientColor);
	void calculateLighting(std::vector<Point>& points, const Color& ambientColor, std::vector<Light>& lights, double A, double B);
	void calcuateDepthShading(std::vector<Point>& points, const Depth& depth);
}