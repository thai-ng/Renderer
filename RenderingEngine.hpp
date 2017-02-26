#pragma once

#include "primitives.h"
#include "lerp.h"

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
	void RenderTriangle(const Triangle_t& triangle, RenderMode renderMode);
	void RenderLine(const Line_t& triangle, RenderMode renderMode);

private:
};