#include "primitives.hpp"

Point::Point(double x, double y, double z, const Rect* parent, const Color& color) : x(x), y(y), z(z), parent(parent), color(color) {}


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

Point operator*(const Point& p, double s)
{
	return Point{ p.x*s, p.y*s, p.z*s, p.parent, p.color };
}

Point operator/(const Point& p, double s)
{
	return Point{ p.x/s, p.y/s, p.z/s, p.parent, p.color };
}

bool Point::operator==(const Point& other) const
{
	return (x == other.x) && (y == other.y) && (z == other.z) && (parent == other.parent);
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
	return Point{ p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
}

Point operator-(const Point& p1, const Point& p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
}

Point4D operator*(const Point4D& p, double s)
{
	return Point4D{ p.x*s, p.y*s, p.z*s, 1.0 };
}

Point4D operator/(const Point4D& p, double s)
{
	return Point4D{ p.x / s, p.y / s, p.z / s, 1.0 };
}

Point4D operator-(const Point4D& x, const Point4D& y)
{
	return Point4D{ x.x - y.x,
		x.y - y.y,
		x.z - y.z,
		1.0 };
}

Point4D operator+(const Point4D& x, const Point4D& y)
{
	return Point4D{ x.x + y.x,
		x.y + y.y,
		x.z + y.z,
		1.0 };
}


double dot(const Point4D& v1, const Point4D& v2)
{
	auto result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return result;
}

Point4D cross(const Point4D& v1, const Point4D& v2)
{
	return Point4D{ v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
		1.0 };
}

double length(const Point4D& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Point4D normalize(const Point4D& v)
{
	auto l = length(v);
	return v / l;
}
