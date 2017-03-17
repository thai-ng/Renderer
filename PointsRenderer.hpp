#pragma once
#include <vector>

#include "drawable.h"
#include "primitives.hpp"
#include "CommonTypeAliases.hpp"

namespace PointsRenderer
{
	void renderPoints(const std::vector<Point>& points, Drawable* drawSurface, Matrix2D<int>& zBuffer, const Rect& viewPort);
}