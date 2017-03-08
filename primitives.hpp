#pragma once
#include <cmath>
#include <tuple>
#include <vector>
#include <array>

#include "Color.hpp"

template <typename T>
using Matrix2D = std::vector<std::vector<T>>;

struct Rect;

struct Point
{
	int x;
	int y;
	int z;
	const Rect* parent;
	Color color;

	Point() : x(0), y(0), parent(nullptr), color(0xffffffff) {}
	Point(int x, int y, int z = 0, const Rect* parent = nullptr, const Color& color = 0xffffffff);

	Point toGlobalCoordinate() const;

	bool operator==(const Point& other) const;

	bool operator!=(const Point& other) const;

	Point flipped() const;
};

struct Point4D
{
	Point4D(const std::array<double, 4>& vector, const Color& color) : x(vector[0]), y(vector[1]), z(vector[2]), w(vector[3]), color(color) {}
	Point4D(double x, double y, double z, double w, const Color& color) : x(x), y(y), z(z), w(w), color(color) {}
	
	std::array<double, 4> getVector()
	{
		return std::array<double, 4> {x, y, z, w};
	}

	double x;
	double y;
	double z;
	double w;
	Color color;
};

struct Rect
{
	int x;
	int y;
	int width;
	int height;

	int bottom() const;
	int right() const;
	Point center() const;
};


Point operator+(const Point& p1, const Point& p2);
Point operator-(const Point& p1, const Point& p2);
double getRadianFromDegree(int angle);

struct Line
{
	Line(Point inp1, Point inp2) : p1(inp1), p2(inp2) {}

	Line(Point inp1, Point inp2, unsigned int c);

	Line(const Point& origin, int length, int angle, Rect* parent = nullptr, unsigned int c = 0xffffffff);

	Point p1;
	Point p2;
	unsigned int color;

	std::tuple<Point, Point> toGlobalCoordinate() const;

	void setParent(Rect* parent);

	bool connected(const Line& other) const;

	std::array<Point, 3> getTriangleVertices(const Line& other) const;
};

class Polygon
{
public:
	explicit Polygon(std::vector<Point>&& v, Rect* parent = nullptr);
	
	std::vector<Point> vertices() const;

	const Rect* parent() const;

private:
	std::vector<Point> _vertices;
	const Rect* _parent;
};

class Triangle
{
public:
	Triangle(const std::array<Point, 3>& vertices, const Rect* parent = nullptr);

	Triangle(const Line& line1, const Line& line2);

	std::array<Point, 3> vertices() const;

	const Rect* parent() const;

private:
	const Rect* _parent;
	std::array<Point, 3> _vertices;
};
