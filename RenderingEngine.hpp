#pragma once
#include <cmath>

#include "Color.hpp"
#include "drawable.h"
#include "lerp.hpp"
#include "Matrix.hpp"
#include "primitives.hpp"

using Vector4_t = std::array<double, 4>;
using Triangle_t = std::array<Vector4_t, 3>;
using Line_t = std::array<Point4D, 2>;

class RenderEngine
{
public:
	RenderEngine(const Rect& viewPort, Drawable* drawSurface, const Color& maxColor);

	enum class RenderMode
	{
		Filled,
		Wireframe
	};

	void RenderTriangle(const Triangle_t& triangle, RenderMode renderMode);

	void RenderLine(const Line_t& line);

private:
	Color getColorFromZ(int z) const;
	Color getColorWithDepth(const Color& baseColor, int z) const;
	Lerp<int> redLerp;
	Lerp<int> greenLerp;
	Lerp<int> blueLerp;
	Matrix2D<int> zBuffer;
	int zThreshold = 200;
	Rect _viewPort;
	Color _maxColor;
	Drawable* _drawSurface;
	Matrix<4, 4, double> viewPortTransformationMatrix = Matrix<4, 4, double>{ 1.0, 0.0, 0.0, 0.0,
																			  0.0, 1.0, 0.0, 0.0,
																			  0.0, 0.0, 1.0, 0.0,
																			  0.0, 0.0, 0.0, 1.0 };
};