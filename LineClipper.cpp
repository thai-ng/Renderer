#include "LineClipper.h"

Point4D getNormal(const Plane_t& plane)
{
	auto pv1 = plane[1] - plane[0];
	auto pv2 = plane[2] - plane[0];
	return cross(pv1, pv2);
}

std::optional<Point4D> intersect(const Plane_t& plane, const Line_t& line)
{
	auto normal = getNormal(plane);
	auto d = dot(normal, plane[0]);

	auto ray = line[1] - line[0];
	if (dot(normal, ray) == 0)
	{
		return {};
	}

	double x = (d - dot(normal, line[0])) / dot(normal, ray);
	return line[0] + (normalize(ray) * x);
}
