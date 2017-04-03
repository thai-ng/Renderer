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

	auto viewPlaneWidth = 200.0;
	auto viewPlaneHeight = 200.0;

	auto scaleMatrix = Matrix<4, 4, double>{ viewPort.width / viewPlaneWidth, 0.0,								  0.0, 0.0,
											 0.0,							  -viewPort.height / viewPlaneHeight, 0.0, 0.0,
											 0.0,							  0.0,								  1.0, 0.0,
											 0.0,							  0.0,								  0.0, 1.0 };

	auto translationMatrix = Matrix<4, 4, double>{ 1.0, 0.0, 0.0, viewPort.width / 2.0,
												   0.0, 1.0, 0.0, viewPort.height / 2.0,
												   0.0, 0.0, 1.0, 0.0,
												   0.0, 0.0, 0.0, 1.0 };

	viewPortTransformationMatrix = viewPortTransformationMatrix * translationMatrix;
	viewPortTransformationMatrix = viewPortTransformationMatrix * scaleMatrix;
}

void RenderEngine::RenderTriangle(const Polygon_t& triangle, RenderMode renderMode)
{
	// Translate to screen space
	std::vector<Point> vertices;
	vertices.resize(triangle.size());
	std::transform(triangle.begin(), triangle.end(), vertices.begin(), [this](auto& p)
	{
		auto v = perspectiveTransformationMatrix * p.getVector();
		v = viewPortTransformationMatrix * v;
		// HACK
		if ((v[3] - 0) > 0.00001)
		{
			v = v / v[3];
		}
		else
		{
			v = v / 0.01;
		}

		return Point{ static_cast<int>(std::round(v[0])), static_cast<int>(std::round(v[1])), static_cast<int>(std::round(v[2])), &this->_viewPort, p.color };
	});

	std::vector<Point> points;
	if (renderMode == RenderMode::Filled)
	{
		points = std::move(PointGenerator::generatePolygonPoints(vertices));
	}
	else
	{
		points = std::move(PointGenerator::generateWireframePoints(vertices));
	}

	PointLighter::calculateAmbientLight(points, ambientColor);

	if (depthSet)
	{
		PointLighter::calcuateDepthShading(points, _depth);
	}

	PointsRenderer::renderPoints(points, _drawSurface, zBuffer, _viewPort, _camera);
}

void RenderEngine::RenderLine(const Line_t& line)
{
	std::vector<Point> vertices;
	vertices.resize(line.size());
	std::transform(line.begin(), line.end(), vertices.begin(), [this](auto& p)
	{
		auto v = perspectiveTransformationMatrix * p.getVector();
		v = viewPortTransformationMatrix * v;
		// HACK		
		if (v[3] != 0)
		{
			v = v / v[3];
		}
		else
		{
			v = v / _camera.near;
		}

		return Point{ static_cast<int>(std::round(v[0])), static_cast<int>(std::round(v[1])), static_cast<int>(std::round(v[2])), &this->_viewPort, p.color };
	});

	auto points = std::move(PointGenerator::generateLinePoints(vertices[0], vertices[1]));
	points.erase(std::remove_if(points.begin(), points.end(), [this](auto& p) { return  p.z > this->_camera.far || p.z < this->_camera.near; }), points.end());
	PointLighter::calculateAmbientLight(points, ambientColor);

	if (depthSet)
	{
		PointLighter::calcuateDepthShading(points, _depth);
	}

	PointsRenderer::renderPoints(points, _drawSurface, zBuffer, _viewPort, _camera);
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
	auto viewPlaneWidth = _camera.xHigh - _camera.xLow;
	auto viewPlaneHeight = _camera.yHigh - _camera.yLow;

	auto scaleMatrix = Matrix<4, 4, double>{ _viewPort.width / viewPlaneWidth, 0.0,								   0.0, 0.0,
											 0.0,							  -_viewPort.height / viewPlaneHeight, 0.0, 0.0,
											 0.0,							   0.0,								   1.0, 0.0,
											 0.0,							   0.0,								   0.0, 1.0 };

	auto translationMatrix = Matrix<4, 4, double>{ 1.0, 0.0, 0.0, _viewPort.width / 2.0,
												   0.0, 1.0, 0.0, _viewPort.height / 2.0,
												   0.0, 0.0, 1.0, 0.0,
												   0.0, 0.0, 0.0, 1.0 };

	viewPortTransformationMatrix = CTM_t{ 1.0, 0.0, 0.0, 0.0,
										  0.0, 1.0, 0.0, 0.0,
										  0.0, 0.0, 1.0, 0.0,
										  0.0, 0.0, 0.0, 1.0 };

	viewPortTransformationMatrix = viewPortTransformationMatrix * translationMatrix;
	viewPortTransformationMatrix = viewPortTransformationMatrix * scaleMatrix;

	for (auto& r : zBuffer)
	{
		for (auto& e : r)
		{
			e = static_cast<int>(std::round(_camera.far + 1));
		}
	}
}

void RenderEngine::SetDepth(const Depth& depth)
{
	depthSet = true;
	_depth = depth;
}