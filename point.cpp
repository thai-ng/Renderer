#include "primitives.hpp"

Point::Point(int x, int y, int z, const Rect* parent, const Color& color) : x(static_cast<int>(x)), y(static_cast<int>(y)), z(static_cast<int>(z)), parent(parent), color(color) {}


Point Point::toGlobalCoordinate() const
{
	if (parent)
	{
		return Point{ parent->x + x, parent->y + y, z, parent, color };
	}
	else
	{
		return Point{ x, y, z, nullptr, color };
	}
}

bool Point::operator==(const Point& other) const
{
	return (x == other.x) && (y == other.y) && (parent == other.parent);
}

bool Point::operator!=(const Point& other) const
{
	return !(*this == other);
}

Point Point::flipped() const
{
	return Point{ y, x, z, parent, color };
}

Point operator+(const Point& p1, const Point& p2)
{
	return Point{ p1.x + p2.x, p1.y + p2.y };
}

Point operator-(const Point& p1, const Point& p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y };
}
