#pragma once

#include <cmath>
#include <array>
#include <optional>

#include "CommonTypeAliases.hpp"
#include "primitives.hpp"

Point4D getNormal(const Plane_t& plane);

std::optional<Point4D> intersect(const Plane_t& plane, const Line_t& line);

template<typename Comp>
std::vector<Point> clipZ(const std::vector<Point>& points, double z, Comp comp)
{
	std::vector<Point> result;
	for (auto p1i = 0u; p1i < points.size(); ++p1i)
	{
		auto p2i = p1i + 1;
		if (p2i == points.size())
		{
			p2i = 0;
		}
		auto p1 = points[p1i];
		auto p2 = points[p2i];

		auto p1t = comp(p1);
		auto p2t = comp(p2);
		
		// both OK
		if (p1 && p2)
		{
			result.push_back(p1, p2);
		}
		// First point OK, find 2nd clip point
		else if (p1)
		{
			result.push_back(p1);
			auto newP2 = Lerp<Point>::lerp(p1, p2, std::abs(z) / std::abs(p1.z - p2.z));
			newP2.z = z;
			result.push_back(newP2);
		}
		// Second point OK, find 1st clip point
		else if (p2)
		{
			auto newP1 = Lerp<Point>::lerp(p1, p2, std::abs(z) / std::abs(p1.z - p2.z));
			newP1.z = z;
			result.push_back(newP1);
			result.push_back(p2);
		}
	}
	
	return result;
}
