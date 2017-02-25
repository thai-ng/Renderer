#pragma once

#include "primitives.h"

class RenderingEngine
{
public:
	enum class RenderMode
	{
		Filled,
		Wireframe
	};

	void RenderPolygon(const Polygon& polygon, RenderMode renderMode);
	void RenderLine(const Line& line, RenderMode renderMode);
};