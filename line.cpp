#include "primitives.h"

Line::Line(Point inp1, Point inp2, unsigned int c) : p1(inp1), p2(inp2), color(c)
{
	p1.color = Color(color);
	p2.color = Color(color);
}

Line::Line(const Point& origin, int length, int angle, Rect* parent, unsigned int c) : p1(origin), color(c)
{
	auto radianAngle = getRadianFromDegree(angle);
	auto opposite = length * std::sin(radianAngle);
	auto adjacent = length * std::cos(radianAngle);
	p2 = Point{ origin.x + static_cast<int>(adjacent), origin.y + static_cast<int>(opposite), origin.z, parent , color };
	if (p1.parent != parent)
	{
		p1.parent = parent;
	}
	p1.color = Color(color);
}

std::tuple<Point, Point> Line::toGlobalCoordinate() const
{
	return std::make_tuple<Point, Point>(p1.toGlobalCoordinate(), p2.toGlobalCoordinate());
}

void Line::setParent(Rect* parent)
{
	p1.parent = parent;
	p2.parent = parent;
}

bool Line::connected(const Line& other) const
{
	return (p1 == other.p1 || p1 == other.p2 || p2 == other.p1 || p2 == other.p2);
}

std::array<Point, 3> Line::getTriangleVertices(const Line& other) const
{
	if (connected(other))
	{
		if (p1 == other.p1)
		{
			return std::array<Point, 3>{p1, p2, other.p2};
		}
		else if (p1 == other.p2)
		{
			return std::array<Point, 3>{other.p1, p2, other.p2};
		}
		else if (p2 == other.p1)
		{
			return std::array<Point, 3>{p1, p2, other.p2};
		}
		else
		{
			return std::array<Point, 3>{p1, p2, other.p1};
		}
	}
	else
	{
		throw std::invalid_argument("Lines must connect.");
	}
}