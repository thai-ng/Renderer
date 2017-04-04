#pragma once

#include <cmath>
#include <array>
#include <optional>

#include "CommonTypeAliases.hpp"
#include "primitives.hpp"

double dot(const Vector4_t& v1, const Vector4_t& v2)
{
	double sum = 0.0;
	for (auto i = 0; i < v1.size(); ++i)
	{
		sum += v1[i] * v2[2];
	}

	return sum;
}

Vector4_t cross(const Vector4_t& v1, const Vector4_t& v2)
{
	return Vector4_t{
		v1[1]*v2[2] - v1[2]*v2[1],
		v1[2]*v2[0] - v1[0]*v2[2],
		v1[0]*v2[1] - v1[1]*v2[0],
		1.0
	};
}

double length(const Vector4_t& v)
{
	return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

Vector4_t normalize(const Vector4_t& v)
{
	auto l = length(v);
	return Vector4_t{ v[0] / l,
					  v[1] / l,
					  v[2] / l,
					  1.0 };
}

Vector4_t operator-(const Vector4_t& x, const Vector4_t& y)
{
	return Vector4_t{ x[0] - y[0],
					  x[1] - y[1],
					  x[2] - y[2],
					  1.0 };
}

Vector4_t operator+(const Vector4_t& x, const Vector4_t& y)
{
	return Vector4_t{ x[0] + y[0],
		              x[1] + y[1],
		              x[2] + y[2],
		              1.0 };
}

Vector4_t operator*(const Vector4_t& x, double s)
{
	return { x[0] * s, x[1] * s, x[2] * s, 1.0 };
}

using Plane_t = std::array<Vector4_t, 3>;
using Line_t = std::array<Vector4_t, 2>;

Vector4_t getNormal(const Plane_t& plane)
{
	auto pv1 = plane[1] - plane[0];
	auto pv2 = plane[2] - plane[0];
	return cross(pv1, pv2);
}

std::optional<Vector4_t> intersect(const Plane_t& plane, const Line_t& line)
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