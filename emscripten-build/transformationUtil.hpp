#pragma once
#include <cmath>
#include <algorithm>

#include "primitives.hpp"

template <typename T>
T rotate(const T& shape, int angle, const Point& center)
{
	auto radianAngle = getRadianFromDegree(angle);
	auto vertices = shape.vertices();
	std::for_each(vertices.begin(), 
			      vertices.end(), 
		          [radianAngle, &center](auto& point) 
				  { 
						const auto x = point.x - center.x;
						const auto y = point.y - center.y;
						const auto c = cos(radianAngle);
						const auto s = sin(radianAngle);
						point.x = static_cast<int>(c * x - s * y) + center.x;
						point.y = static_cast<int>(s * x + c * y) + center.y;
				  });
	return T(vertices, shape.parent());
}