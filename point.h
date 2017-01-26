#pragma once
#include "rect.h"

struct Point
{
	int x;
	int y;
	Rect* parent;

	auto toGlobalCoordinate() const
	{
		if (parent)
		{
			return Point{ parent->x + x, parent->y + y };
		}
		else
		{
			return Point{ x, y };
		}
	}
};

Point operator+(const Point& p1, const Point& p2)
{
	return Point{ p1.x + p2.x, p1.y + p2.y };
}

Point operator-(const Point& p1, const Point& p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y };
}
