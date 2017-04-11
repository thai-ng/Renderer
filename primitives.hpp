#pragma once
#include <cmath>
#include <tuple>
#include <vector>
#include <array>
#include <optional>
#include <numeric>

#include "Color.hpp"

template <typename T>
using Matrix2D = std::vector<std::vector<T>>;

struct Rect;

struct Point
{
	double x;
	double y;
	double z;
	const Rect* parent;
	Color color;

	Point() : x(0.0), y(0.0), z(0.0), parent(nullptr), color(0xffffffff) {}
	Point(double x, double y, double z = 0.0, const Rect* parent = nullptr, const Color& color = 0xffffffff);
	Point(const std::array<double, 3> &p) : x(p[0]), y(p[1]), z(p[2]), parent(nullptr), color(0xffffffff) {}

	Point toGlobalCoordinate() const;

	bool operator==(const Point& other) const;

	bool operator!=(const Point& other) const;

	Point flipped() const;
};

Point operator*(const Point& p, double s);
Point operator+(const Point& p1, const Point& p2);
Point operator/(const Point& p, double s);

struct Point4D
{
	Point4D() : x(0), y(0), z(0), w(0), color(0xffffffff) {}
	Point4D(const std::array<double, 4>& vector, const Color& color) : x(vector[0]), y(vector[1]), z(vector[2]), w(vector[3]), color(color) {}
	Point4D(double x, double y, double z, double w, const Color& color) : x(x), y(y), z(z), w(w), color(color) {}
	Point4D(double x, double y, double z, double w, const Color& color, const Point& normal) : x(x), y(y), z(z), w(w), color(color), normal(normal) {}
	Point4D(double x, double y, double z, double w) : x(x), y(y), z(z), w(w), color(0xffffffff) {}
	Point4D(const Point& p) : x(p.x), y(p.y), z(p.z), w(1.0), color(p.color) {}

	Point4D& operator=(const Point4D& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		color = other.color;
		normal = other.normal;

		return *this;
	}

	std::array<double, 4> getVector() const
	{
		return std::array<double, 4> {x, y, z, w};
	}

	double x;
	double y;
	double z;
	double w;
	Color color;
	std::optional<Point> normal = std::nullopt;
	std::optional<Point> cameraSpacePoint = std::nullopt;
};

Point4D operator*(const Point4D& p, double s);
Point4D operator/(const Point4D& p, double s);
Point4D operator-(const Point4D& x, const Point4D& y);
Point4D operator+(const Point4D& x, const Point4D& y);

double dot(const Point4D& v1, const Point4D& v2);
Point4D cross(const Point4D& v1, const Point4D& v2);

double length(const Point4D& v);
Point4D normalize(const Point4D& v);

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
	Line(Point inp1, Point inp2) : p1(inp1), p2(inp2), color(0xffffffff) {}

	Line(Point inp1, Point inp2, unsigned int c);

	Line(const Point& origin, int length, int angle, Rect* parent = nullptr, unsigned int c = 0xffffffff);

	Point p1;
	Point p2;
	unsigned int color;

	std::tuple<Point, Point> toGlobalCoordinate() const;

	void setParent(const Rect* parent);

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

enum class LightingMethod
{
	Phong,
	Gouraud,
	Flat
};


template <typename T>
T getCenterPoint(const std::vector<T>& points)
{
	auto centerPoint = std::accumulate(points.begin(), points.end(), T{ 0.0, 0.0, 0.0, 1.0, 0xffffffff });
	centerPoint = centerPoint / static_cast<double>(points.size());
	return centerPoint;
}

template <typename T>
T sortVertices(const T& vertices)
{
	T sortedVertices{ vertices };
	auto center = getCenterPoint(vertices);
	std::sort(sortedVertices.begin(), sortedVertices.end(), [&center](const auto& a, const auto& b)
	{
		if (a.x - center.x >= 0 && b.x - center.x < 0)
			return false;
		if (a.x - center.x < 0 && b.x - center.x >= 0)
			return true;
		if (a.x - center.x == 0 && b.x - center.x == 0) {
			if (a.y - center.y >= 0 || b.y - center.y >= 0)
				return a.y < b.y;
			return b.y < a.y;
		}

		// compute the cross product of vectors (center -> a) x (center -> b)
		auto det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
		if (det < 0)
			return false;
		if (det > 0)
			return true;

		// points a and b are on the same line from the center
		// check which point is closer to the center
		auto d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
		auto d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
		return d1 < d2;
	});
	return sortedVertices;
}
