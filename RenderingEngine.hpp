#pragma once
#include <cmath>

#include "Camera.hpp"
#include "Color.hpp"
#include "drawable.h"
#include "lerp.hpp"
#include "primitives.hpp"
#include "CommonTypeAliases.hpp"

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

	void SetAmbientColor(const Color& color);

	void SetCamera(const Camera& camera);

private:
	Color getColorFromZ(int z) const;
	Color getColorWithDepth(const Color& baseColor, int z) const;
	
	Lerp<int> redLerp;
	Lerp<int> greenLerp;
	Lerp<int> blueLerp;
	
	Matrix2D<int> zBuffer;
	int zThreshold = 200;
	
	Rect _viewPort;
	
	Color ambientColor = Color(0, 0, 0);
	
	Drawable* _drawSurface;
	
	Camera _camera;
	
	CTM_t viewPortTransformationMatrix = CTM_t { 1.0, 0.0, 0.0, 0.0,
												 0.0, 1.0, 0.0, 0.0,
												 0.0, 0.0, 1.0, 0.0,
												 0.0, 0.0, 0.0, 1.0 };
};