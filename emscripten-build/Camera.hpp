#pragma once
#include "CommonTypeAliases.hpp"

struct Camera
{
	CTM_t transformationMatrix;
	double xLow;
	double xHigh;
	double yLow;
	double yHigh;
	double near;
	double far;
};