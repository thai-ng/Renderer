#pragma once
#include <cmath>

#include "Camera.hpp"
#include "Color.hpp"
#include "CommonTypeAliases.hpp"
#include "Depth.hpp"
#include "drawable.h"
#include "lerp.hpp"
#include "Light.hpp"
#include "primitives.hpp"
#include "Face.hpp"

class RenderEngine
{
public:
	RenderEngine(const Rect& viewPort, Drawable* drawSurface, const Color& maxColor);

	enum class RenderMode
	{
		Filled,
		Wireframe
	};

	void RenderTriangle(const Polygon_t& triangle, RenderMode renderMode);

	void RenderFace(const Face& triangle, RenderMode renderMode);

	void RenderLine(const Line_t& line);

	void SetAmbientColor(const Color& color);

	void SetCamera(const Camera& camera);

	void SetDepth(const Depth& depth);

	void AddLight(const Light& light);

	void SetLightingMethod(const LightingMethod& lightingMethod);

	void SetSpecularCoefficient(double value);
	
	void SetSpecularExponent(double value);

private:
	Matrix2D<double> zBuffer;
	double zThreshold = std::numeric_limits<double>::max();
	
	Rect _viewPort;
	
	Color ambientColor = Color(0, 0, 0);
	
	bool depthSet = false;

	Drawable* _drawSurface;
	
	Camera _camera;
	Depth _depth = Depth{ 0, std::numeric_limits<double>::max(), Color{0, 0, 0} };
	CTM_t viewPortTransformationMatrix = CTM_t { 1.0, 0.0, 0.0, 0.0,
												 0.0, 1.0, 0.0, 0.0,
												 0.0, 0.0, 1.0, 0.0,
												 0.0, 0.0, 0.0, 1.0 };


	CTM_t perspectiveTransformationMatrix = CTM_t{ 1.0, 0.0, 0.0,  0.0,
												   0.0, 1.0, 0.0,  0.0,
												   0.0, 0.0, 1.0,  0.0,
												   0.0, 0.0, 1.0,  0.0 };

	std::vector<Light> lights;
	LightingMethod currentLightingMethod = LightingMethod::Flat;

	Plane_t nearPlane;
	//Plane_t farPlane;

	double ks = .3;
	double p = 8;
};
