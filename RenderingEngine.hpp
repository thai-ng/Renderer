#pragma once
#include <cmath>

#include "primitives.h"
#include "lerp.h"
#include "Color.h"

using Vector4_t = std::array<double, 4>;
using Triangle_t = std::array<Vector4_t, 3>;
using Line_t = std::array<Vector4_t, 2>;

class RenderEngine
{
public:
	enum class RenderMode
	{
		Filled,
		Wireframe
	};
	void RenderTriangle(const Triangle_t& triangle, RenderMode renderMode)
	{
		// Translate to screen space


		Point point1 = Point{ static_cast<int>(std::round(triangle[0][0])), static_cast<int>(std::round(triangle[0][1])), static_cast<int>(std::round(triangle[0][2])), nullptr, getColorFromZ(static_cast<int>(std::round(triangle[0][2]))) };
		Point point2 = Point{ static_cast<int>(std::round(triangle[1][0])), static_cast<int>(std::round(triangle[1][1])), static_cast<int>(std::round(triangle[1][2])), nullptr, getColorFromZ(static_cast<int>(std::round(triangle[0][2]))) };
		Point point3 = Point{ static_cast<int>(std::round(triangle[2][0])), static_cast<int>(std::round(triangle[2][1])), static_cast<int>(std::round(triangle[2][2])), nullptr, getColorFromZ(static_cast<int>(std::round(triangle[0][2]))) };

		if (renderMode == RenderMode::Filled)
		{
			renderPolygon(std::vector<Point>{ point1, point2, point3 }, nullptr, Color{ 0, 0, 0 }, &zBuffer);
		}
		else
		{
			renderPolygonWireframe(std::vector<Point>{ point1, point2, point3 }, nullptr, &zBuffer);
		}
	}
	void RenderLine(const Line_t& line, RenderMode renderMode)
	{
		// Translate to screen space

		// Assign z Color
		Point point1 = Point{ static_cast<int>(std::round(line[0][0])), static_cast<int>(std::round(line[0][1])), static_cast<int>(std::round(line[0][2])), nullptr, getColorFromZ(static_cast<int>(std::round(line[0][2]))) };
		Point point2 = Point{ static_cast<int>(std::round(line[1][0])), static_cast<int>(std::round(line[1][1])), static_cast<int>(std::round(line[1][2])), nullptr, getColorFromZ(static_cast<int>(std::round(line[0][2]))) };

		DDALineRenderer(point1, point2, nullptr, Color(0, 0, 0), 1.0, &zBuffer);
	}

private:
	Color getColorFromZ(int z) const
	{
		auto r = static_cast<unsigned char>(redLerp[z].second);
		auto g = static_cast<unsigned char>(greenLerp[z].second);
		auto b = static_cast<unsigned char>(blueLerp[z].second);
		return Color{ r, g, b };
	}


	Lerp<int> redLerp  = Lerp<int>(0, 200, 255, 0);
	Lerp<int> greenLerp = Lerp<int>(0, 200, 255, 0);
	Lerp<int> blueLerp = Lerp<int>(0, 200, 255, 0);
	Matrix2D<int> zBuffer;
	int zThreshold = 200;

};