#pragma once
#include "point.h"
#include "rect.h"
#include <tuple>

struct Line
{
	Point p1;
	Point p2;
	Rect* parent;

        auto toGlobalCoordinate() const
	{
		if (parent)
		{
			return std::make_tuple<Point, Point>(Point{ parent->x + p1.x, parent->y + p1.y }, Point{ parent->x + p2.x, parent->y + p2.y });
		}
		else
		{
                        return std::make_tuple<Point, Point>(Point{ p1.x, p1.y }, Point{ p2.x, p2.y });
		}
	}
};
