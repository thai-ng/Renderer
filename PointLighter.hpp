#pragma once
#include <vector>
#include "primitives.hpp"
#include "Depth.hpp"
#include "Light.hpp"

namespace PointLighter
{
	void calculateAmbientLight(std::vector<Point4D>& points, const Color& ambientColor);
	void calculateLighting(std::vector<Point4D>& points, const Color& ambientColor, std::vector<Light>& lights, double ks, double kp);
	Color calculateLightAtPixel(const Point4D& p, const Point4D& vN, const Light& l, double ks, double kp);
	Color calculateLights(Point4D & p, std::vector<Light> & lights, double ks, double kp);
	void calculateDepthShading(std::vector<Point4D>& points, const Depth& depth);
}