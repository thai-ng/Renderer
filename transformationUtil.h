#pragma once
#include <cmath>
#include <algorithm>

#include "primitives.h"

template <typename T>
T rotate(const T& shape, int angle, const Point& center)
{
	auto radianAngle = getRadianFromDegree(angle);
	auto vertices = shape.vertices();
	std::for_each(vertices.begin(), 
			      vertices.end(), 
		          [radianAngle, &center](auto& point) 
				  { 
						auto x = point.x - center.x;
						auto y = point.y - center.y;
						auto c = cos(radianAngle);
						auto s = sin(radianAngle);
						point.x = static_cast<int>(c * x - s * y) + center.x;
						point.y = static_cast<int>(s * x + c * y) + center.y;
				  });
	return T(vertices, shape.parent());
}