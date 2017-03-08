#include "RenderingEngine.hpp"
#include "Matrix.hpp"
#include "lineRenderer.hpp"
#include "polygonRenderer.hpp"

RenderEngine::RenderEngine(const Rect& viewPort, Drawable* drawSurface, const Color& maxColor) :
	_viewPort(viewPort),
	_drawSurface(drawSurface),
	redLerp(0, 200, std::get<0>(maxColor.getColorChannels()), 0),
	greenLerp(0, 200, std::get<1>(maxColor.getColorChannels()), 0),
	blueLerp(0, 200, std::get<2>(maxColor.getColorChannels()), 0),
	_maxColor(maxColor)
{
	zBuffer = Matrix2D<int>(_viewPort.width, std::vector<int>(_viewPort.height, zThreshold));

	auto scaleMatrix = Matrix<4, 4, double>{ viewPort.width / 200.0, 0.0,					  0.0, 0.0,
											 0.0,				     -viewPort.height / 200.0, 0.0, 0.0,
											 0.0,				     0.0,					  1.0, 0.0,
											 0.0,				     0.0,					  0.0, 1.0 };

	auto translationMatrix = Matrix<4, 4, double>{ 1.0, 0.0, 0.0, viewPort.width / 2.0,
												   0.0, 1.0, 0.0, viewPort.height / 2.0,
												   0.0, 0.0, 1.0, 0.0,
												   0.0, 0.0, 0.0, 1.0 };

	viewPortTransformationMatrix = viewPortTransformationMatrix * translationMatrix;
	viewPortTransformationMatrix = viewPortTransformationMatrix * scaleMatrix;
}

void RenderEngine::RenderTriangle(const Triangle_t& triangle, RenderMode renderMode)
{
	// Translate to screen space
	auto v1 = triangle[0];
	v1 = viewPortTransformationMatrix * v1;
	auto v2 = triangle[1];
	v2 = viewPortTransformationMatrix * v2;
	auto v3 = triangle[2];
	v3 = viewPortTransformationMatrix * v3;

	Point point1 = Point{ static_cast<int>(std::round(v1[0])), static_cast<int>(std::round(v1[1])), static_cast<int>(std::round(v1[2])), &_viewPort, getColorFromZ(static_cast<int>(std::round(v1[2]))) };
	Point point2 = Point{ static_cast<int>(std::round(v2[0])), static_cast<int>(std::round(v2[1])), static_cast<int>(std::round(v2[2])), &_viewPort, getColorFromZ(static_cast<int>(std::round(v2[2]))) };
	Point point3 = Point{ static_cast<int>(std::round(v3[0])), static_cast<int>(std::round(v3[1])), static_cast<int>(std::round(v3[2])), &_viewPort, getColorFromZ(static_cast<int>(std::round(v3[2]))) };

	if (renderMode == RenderMode::Filled)
	{
		renderPolygon(Polygon{ std::vector<Point>{ point1, point2, point3 }, &_viewPort }, _drawSurface, 1.0, &zBuffer);
	}
	else
	{
		renderPolygonWireframe(Polygon{ std::vector<Point>{ point1, point2, point3 }, &_viewPort }, _drawSurface, &zBuffer);
	}
}

void RenderEngine::RenderLine(const Line_t& line)
{
	// Translate to screen space
	auto p1 = line[0];
	auto v1 = viewPortTransformationMatrix * p1.getVector();
	auto p2 = line[1];
	auto v2 = viewPortTransformationMatrix * p2.getVector();

	// Assign z Color
	Point point1 = Point{ static_cast<int>(std::round(v1[0])), static_cast<int>(std::round(v1[1])), static_cast<int>(std::round(v1[2])), &_viewPort, getColorWithDepth(p1.color, static_cast<int>(std::round(v1[2]))) };
	Point point2 = Point{ static_cast<int>(std::round(v2[0])), static_cast<int>(std::round(v2[1])), static_cast<int>(std::round(v2[2])), &_viewPort, getColorWithDepth(p2.color, static_cast<int>(std::round(v2[2]))) };

	// This takes global coordinate, fix first
	point1 = point1.toGlobalCoordinate();
	point2 = point2.toGlobalCoordinate();
	renderLine(point1, point2, _drawSurface, DDALineRenderer, 1.0, &zBuffer, &_viewPort);
}

Color RenderEngine::getColorFromZ(int z) const
{
	if (z >= 0 && z < 200)
	{
		auto r = static_cast<unsigned char>(redLerp[z].second);
		auto g = static_cast<unsigned char>(greenLerp[z].second);
		auto b = static_cast<unsigned char>(blueLerp[z].second);
		return Color{ r, g, b };
	}
	else if (z < 0)
	{
		return _maxColor;
	}
	else
	{
		return Color{ 0, 0, 0 };
	}
}

Color RenderEngine::getColorWithDepth(const Color& baseColor, int z) const
{
	if (z >= 0 && z < 200)
	{
		Lerp<int> rLerp(0, 200, std::get<0>(baseColor.getColorChannels()), 0);
		Lerp<int> gLerp(0, 200, std::get<1>(baseColor.getColorChannels()), 0);
		Lerp<int> bLerp(0, 200, std::get<2>(baseColor.getColorChannels()), 0);

		auto r = static_cast<unsigned char>(rLerp[z].second);
		auto g = static_cast<unsigned char>(gLerp[z].second);
		auto b = static_cast<unsigned char>(bLerp[z].second);
		return Color{ r, g, b };
	}
	else if (z < 0)
	{
		return baseColor;
	}
	else
	{
		return Color{ 0, 0, 0 };
	}
}
