#pragma once
#include <cmath>
#include <tuple>

struct Rect
{
	int x;
	int y;
	int width;
	int height;

	int bottom() { return y + height; }
	int right() { return x + width; }
	auto center();
};


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

auto Rect::center() 
{ 
	return Point{ (width / 2), (height / 2), this}; 
}

Point operator+(const Point& p1, const Point& p2)
{
	return Point{ p1.x + p2.x, p1.y + p2.y };
}

Point operator-(const Point& p1, const Point& p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y };
}


static constexpr double Pi = 3.141592653589793238462643383279502884;

struct Line
{
	Line(Point inp1, Point inp2, unsigned int c = 0xffffffff) : p1(inp1), p2(inp2), color(c) {}

	Line(const Point& origin, int length, int angle, Rect* parent = nullptr, unsigned int c = 0xffffffff) : p1(origin), color(c)
	{
		auto radianAngle = angle * Pi / 180.0;
		auto opposite = length * std::sin(radianAngle);
		auto adjacent = length * std::cos(radianAngle);
        p2 = Point{ origin.x + static_cast<int>(adjacent), origin.y + static_cast<int>(opposite), parent };
		p1.parent = parent;
	}

	Point p1;
	Point p2;
	unsigned int color;
	auto toGlobalCoordinate() const
	{
		return std::make_tuple<Point, Point>(p1.toGlobalCoordinate(), p2.toGlobalCoordinate());
	}

	void setParent(Rect* parent)
	{
		p1.parent = parent;
		p2.parent = parent;
	}
};

class Polygon
{
public:
	Polygon(std::vector<Point>&& v, Rect* parent = nullptr) : _vertices(v), _parent(parent)
	{
		std::for_each(_vertices.begin(), _vertices.end(), [parent](Point& p) {p.parent = parent; });
	}

	std::vector<Point> vertices() const
	{
		return _vertices;
	}
private:
	std::vector<Point> _vertices;
	Rect* _parent;
};

