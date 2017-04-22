#pragma once

#include "CommonTypeAliases.hpp"
#include "Color.hpp"

struct Light
{
	Vector4_t Location;
	Color color;
	double A;
	double B;
};