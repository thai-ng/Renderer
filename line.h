#pragma once
#include <tuple>
#include "point.h"

struct Line
{
	Point p1;
	Point p2;
	
    auto toGlobalCoordinate() const
	{
		return std::make_tuple<Point, Point>(p1.toGlobalCoordinate(), p2.toGlobalCoordinate());
	}
};
