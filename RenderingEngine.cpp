#include "RenderingEngine.hpp"
#include "Matrix.hpp"
#include "PointGenerator.hpp"
#include "PointsRenderer.hpp"
#include "PointLighter.hpp"

//#include "lineRenderer.hpp"
//#include "polygonRenderer.hpp"

RenderEngine::RenderEngine(const Rect& viewPort, Drawable* drawSurface, const Color& maxColor) :
	_viewPort(viewPort),
	_drawSurface(drawSurface),
	redLerp(0, 200, std::get<0>(maxColor.getColorChannels()), 0),
	greenLerp(0, 200, std::get<1>(maxColor.getColorChannels()), 0),
	blueLerp(0, 200, std::get<2>(maxColor.getColorChannels()), 0),
	ambientColor(maxColor)
{
	zBuffer = Matrix2D<int>(_viewPort.width, std::vector<int>(_viewPort.height, zThreshold));

	auto scaleMatrix = Matrix<4, 4, double>{ viewPort.width / 200.0, 0.0,					   0.0, 0.0,
											 0.0,				     -viewPort.height / 200.0, 0.0, 0.0,
											 0.0,				     0.0,					   1.0, 0.0,
											 0.0,				     0.0,					   0.0, 1.0 };

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
	auto p1 = triangle[0];
	auto v1 = perspectiveTransformationMatrix * p1.getVector();
	v1 = viewPortTransformationMatrix * v1;
	v1 = v1 / v1[3];
	
	auto p2 = triangle[1];
	auto v2 = perspectiveTransformationMatrix * p2.getVector();
	v2 = viewPortTransformationMatrix * v2;
	v2 = v2 / v2[3];
	
	auto p3 = triangle[2];
	auto v3 = perspectiveTransformationMatrix * p3.getVector();
	v3 = viewPortTransformationMatrix * v3;
	v3 = v3 / v3[3];

	Point point1 = Point{ static_cast<int>(std::round(v1[0])), static_cast<int>(std::round(v1[1])), static_cast<int>(std::round(v1[2])), &_viewPort, p1.color };
	Point point2 = Point{ static_cast<int>(std::round(v2[0])), static_cast<int>(std::round(v2[1])), static_cast<int>(std::round(v2[2])), &_viewPort, p2.color };
	Point point3 = Point{ static_cast<int>(std::round(v3[0])), static_cast<int>(std::round(v3[1])), static_cast<int>(std::round(v3[2])), &_viewPort, p3.color };

	std::vector<Point> points;
	if (renderMode == RenderMode::Filled)
	{
		points = std::move(PointGenerator::generatePolygonPoints(std::vector<Point>{point1, point2, point3}));
	}
	else
	{
		points = std::move(PointGenerator::generateWireframePoints(std::vector<Point>{point1, point2, point3}));
	}

	PointLighter::calculateAmbientLight(points, ambientColor);
	PointLighter::calcuateDepthShading(points, _depth);
	PointsRenderer::renderPoints(points, _drawSurface, zBuffer, _viewPort);
}

void RenderEngine::RenderLine(const Line_t& line)
{
	// Translate to screen space
	auto p1 = line[0];
	auto v1 = viewPortTransformationMatrix * p1.getVector();
	auto p2 = line[1];
	auto v2 = viewPortTransformationMatrix * p2.getVector();

	// Assign z Color
	Point point1 = Point{ static_cast<int>(std::round(v1[0])), static_cast<int>(std::round(v1[1])), static_cast<int>(std::round(v1[2])), &_viewPort, p1.color };
	Point point2 = Point{ static_cast<int>(std::round(v2[0])), static_cast<int>(std::round(v2[1])), static_cast<int>(std::round(v2[2])), &_viewPort, p2.color };

	point1 = point1.toGlobalCoordinate();
	point2 = point2.toGlobalCoordinate();
	auto points = std::move(PointGenerator::generateLinePoints(point1, point2));
	PointLighter::calculateAmbientLight(points, ambientColor);
	PointLighter::calcuateDepthShading(points, _depth);
	PointsRenderer::renderPoints(points, _drawSurface, zBuffer, _viewPort);
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
		return ambientColor;
	}
	else
	{
		return Color{ 0, 0, 0 };
	}
}

Color RenderEngine::getColorWithDepth(const Color& baseColor, int z) const
{
	if (z >= _depth.near && z < _depth.far)
	{
		Lerp<double> rLerp(_depth.near, _depth.far, std::get<0>(baseColor.getColorChannels()), 0);
		Lerp<double> gLerp(_depth.near, _depth.far, std::get<1>(baseColor.getColorChannels()), 0);
		Lerp<double> bLerp(_depth.near, _depth.far, std::get<2>(baseColor.getColorChannels()), 0);

		auto r = static_cast<unsigned char>(rLerp[z].second);
		auto g = static_cast<unsigned char>(gLerp[z].second);
		auto b = static_cast<unsigned char>(bLerp[z].second);
		return Color{ r, g, b };
	}
	else if (z < _depth.near)
	{
		return baseColor;
	}
	else
	{
		return _depth.color;
	}
}

void RenderEngine::SetAmbientColor(const Color& color)
{
	ambientColor = color;
}

void RenderEngine::SetCamera(const Camera& camera)
{
	_camera = camera;
}

void RenderEngine::SetDepth(const Depth& depth)
{
	_depth = depth;
}