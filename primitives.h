#pragma once
#include <cmath>
#include <tuple>
#include <vector>
#include <array>

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

	bool operator==(const Point& other) const
	{
		return (x == other.x) && (y == other.y) && (parent == other.parent);
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

	bool connected(const Line& other) const
	{
		return (p1 == other.p1 || p1 == other.p2 || p2 == other.p1 || p2 == other.p2);
	}

	std::array<Point, 3> getTriangleVertices(const Line& other) const
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

class Triangle
{
public:
	Triangle(const std::array<Point, 3>& vertices, Rect* parent = nullptr) : _vertices(vertices), _parent(parent) 
	{
		std::for_each(_vertices.begin(), _vertices.end(), [](auto& vertex) {vertex.parent = _parent; });
	}

	Triangle(const Line& line1, const Line& line2)
	{
		if (line1.connected(line2))
		{
			_vertices = line1.getTriangleVertices(line2);
			_parent = line1.p1.parent;
		}
		else
		{
			throw std::invalid_argument("Lines must connect.");
		}
	}

	auto vertices() const
	{
		return _vertices;
	}
private:
	Rect* _parent;
	std::array<Point, 3> _vertices;
};