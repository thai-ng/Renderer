#include "polygonRenderer.hpp"

#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>

#include "lineRenderer.hpp"

auto comparePoints = [](auto p1, auto p2)
{
	if (p1.y == p2.y)
	{
		return p1.x < p2.x;
	}
	else
	{
		return p1.y < p2.y;
	}
};

double getXSlope(Line line)
{
	// -
	if (line.p1.y == line.p2.y)
	{
		return 0.0;
	}
	else
	{
		// |
		if (line.p1.x == line.p2.x)
		{
			return 0.0;
		}
		else
		{
			return static_cast<double>(line.p2.x - line.p1.x) / static_cast<double>(line.p2.y - line.p1.y);
		}
	}
}

void renderPolygon(const std::vector<Point>& points, Drawable* drawable, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr)
{
	auto sortedVertices = sortVertices(points, comparePoints);
	auto topPoint = sortedVertices.front();
	auto bottomPoint = sortedVertices.back();

	auto vertexChains = splitVertices(sortedVertices, comparePoints);

	// Left line chain
	auto leftChain = std::get<0>(vertexChains);
	auto leftIter = std::next(leftChain.begin());
	Line leftLine{ leftChain.front(), *leftIter };
	Lerp<int> leftLerp(leftLine.p1.y, leftLine.p2.y, leftLine.p1.x, leftLine.p2.x);
	Lerp<int> leftZLerp(leftLine.p1.y, leftLine.p2.y, leftLine.p1.z, leftLine.p2.z);

	auto leftColorLerps = getColorLerp(leftLine.p1.flipped(), leftLine.p2.flipped());
	auto leftRedLerp = std::get<0>(leftColorLerps);
	auto leftGreenLerp = std::get<1>(leftColorLerps);
	auto leftBlueLerp = std::get<2>(leftColorLerps);

	auto leftCount = 0;

	// Right line chain
	auto rightChain = std::get<1>(vertexChains);
	auto rightIter = std::next(rightChain.begin());
	Line rightLine{ rightChain.front(), *rightIter };
	Lerp<int> rightLerp(rightLine.p1.y, rightLine.p2.y, rightLine.p1.x, rightLine.p2.x);
	Lerp<int> rightZLerp(rightLine.p1.y, rightLine.p2.y, rightLine.p1.z, rightLine.p2.z);

	auto rightColorLerps = getColorLerp(rightLine.p1.flipped(), rightLine.p2.flipped());
	auto rightRedLerp = std::get<0>(rightColorLerps);
	auto rightGreenLerp = std::get<1>(rightColorLerps);
	auto rightBlueLerp = std::get<2>(rightColorLerps);

	auto rightCount = 0;

	for (auto y = topPoint.y; y <= bottomPoint.y; ++y)
	{
		auto xl = leftLerp[leftCount].second;
		auto zl = leftZLerp[leftCount].second;
		auto r = static_cast<unsigned char>(leftRedLerp[leftCount].second);
		auto g = static_cast<unsigned char>(leftGreenLerp[leftCount].second);
		auto b = static_cast<unsigned char>(leftBlueLerp[leftCount].second);
		++leftCount;
		auto leftColor = Color(r, g, b);

		auto xr = rightLerp[rightCount].second;
		auto zr = rightZLerp[rightCount].second;

		r = static_cast<unsigned char>(rightRedLerp[rightCount].second);
		g = static_cast<unsigned char>(rightGreenLerp[rightCount].second);
		b = static_cast<unsigned char>(rightBlueLerp[rightCount].second);
		++rightCount;
		auto rightColor = Color(r, g, b);

		auto leftPoint = Point{ static_cast<int>(std::round(xl)), y, static_cast<int>(std::round(zl)), topPoint.parent, leftColor };
		auto rightPoint = Point{ static_cast<int>(std::round(xr)), y,  static_cast<int>(std::round(zr)), topPoint.parent, rightColor };

		renderLine(Line{ leftPoint, rightPoint }, drawable, DDALineRenderer, opacity, zBuffer);

		if (y == leftLine.p2.y && leftLine.p2 != bottomPoint)
		{
			leftIter = std::next(leftIter);
			leftLine = Line{ leftLine.p2, *leftIter };
			leftLerp = Lerp<int>(leftLine.p1.y, leftLine.p2.y, leftLine.p1.x, leftLine.p2.x);
			leftZLerp = Lerp<int>(leftLine.p1.y, leftLine.p2.y, leftLine.p1.z, leftLine.p2.z);

			leftColorLerps = getColorLerp(leftLine.p1.flipped(), leftLine.p2.flipped());
			leftRedLerp = std::get<0>(leftColorLerps);
			leftGreenLerp = std::get<1>(leftColorLerps);
			leftBlueLerp = std::get<2>(leftColorLerps);

			leftCount = 0;
		}

		if (y == rightLine.p2.y && rightLine.p2 != bottomPoint)
		{
			rightIter = std::next(rightIter);
			rightLine = Line{ rightLine.p2, *rightIter };
			rightLerp = Lerp<int>(rightLine.p1.y, rightLine.p2.y, rightLine.p1.x, rightLine.p2.x);
			rightZLerp = Lerp<int>(rightLine.p1.y, rightLine.p2.y, rightLine.p1.z, rightLine.p2.z);


			rightColorLerps = getColorLerp(rightLine.p1.flipped(), rightLine.p2.flipped());
			rightRedLerp = std::get<0>(rightColorLerps);
			rightGreenLerp = std::get<1>(rightColorLerps);
			rightBlueLerp = std::get<2>(rightColorLerps);
			rightCount = 0;
		}
	}
}

void renderPolygon(const std::vector<Point>& points, Drawable* drawable, const Color& color, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr)
{
	auto sortedVertices = sortVertices(points, comparePoints);
	auto topPoint = sortedVertices.front();
	auto bottomPoint = sortedVertices.back();

	auto vertexChains = splitVertices(sortedVertices, comparePoints);

	// Left line chain
	auto leftChain = std::get<0>(vertexChains);
	auto leftIter = std::next(leftChain.begin());
	Line leftLine{ leftChain.front(), *leftIter };
	Lerp<int> leftLerp(leftLine.p1.y, leftLine.p2.y, leftLine.p1.x, leftLine.p2.x);

	auto leftCount = 0;

	// Right line chain
	auto rightChain = std::get<1>(vertexChains);
	auto rightIter = std::next(rightChain.begin());
	Line rightLine{ rightChain.front(), *rightIter };
	Lerp<int> rightLerp(rightLine.p1.y, rightLine.p2.y, rightLine.p1.x, rightLine.p2.x);

	auto rightCount = 0;

	for (auto y = topPoint.y; y <= bottomPoint.y; ++y)
	{
		auto xl = leftLerp[leftCount].second;
		++leftCount;

		auto xr = rightLerp[rightCount].second;
		++rightCount;

		auto leftPoint = Point{ static_cast<int>(std::round(xl)), y, topPoint.z, topPoint.parent, color };
		auto rightPoint = Point{ static_cast<int>(std::round(xr)), y, topPoint.z, topPoint.parent, color };

		renderLine(Line{ leftPoint, rightPoint }, drawable, DDALineRenderer, opacity, zBuffer);

		if (y == leftLine.p2.y && leftLine.p2 != bottomPoint)
		{
			leftIter = std::next(leftIter);
			leftLine = Line{ leftLine.p2, *leftIter };
			leftLerp = Lerp<int>(leftLine.p1.y, leftLine.p2.y, leftLine.p1.x, leftLine.p2.x);

			leftCount = 0;
		}

		if (y == rightLine.p2.y && rightLine.p2 != bottomPoint)
		{
			rightIter = std::next(rightIter);
			rightLine = Line{ rightLine.p2, *rightIter };
			rightLerp = Lerp<int>(rightLine.p1.y, rightLine.p2.y, rightLine.p1.x, rightLine.p2.x);

			rightCount = 0;
		}
	}
}

void renderPolygon(const Polygon& polygon, Drawable* drawable, double opacity, Matrix2D<int>* zBuffer)
{
	renderPolygon(polygon.vertices(), drawable, opacity, zBuffer);
}


void renderPolygon(const Polygon& polygon, Drawable* drawable, const Color& color, Matrix2D<int>* zBuffer)
{
	renderPolygon(polygon.vertices(), drawable, color, 1.0, zBuffer);
}

void renderTriangle(const Triangle& triangle, Drawable* drawable, const Color& color, double opacity, Matrix2D<int>* zBuffer)
{
	auto vertices = triangle.vertices();
	renderPolygon(std::vector<Point>(vertices.begin(), vertices.end()), drawable, color, opacity, zBuffer);
}

void renderTriangle(const Triangle& triangle, Drawable* drawable, double opacity, Matrix2D<int>* zBuffer)
{
	auto vertices = triangle.vertices();
	renderPolygon(std::vector<Point>(vertices.begin(), vertices.end()), drawable, opacity, zBuffer);
}

void renderPolygonWireframe(const std::vector<Point>& points, Drawable* drawable, Matrix2D<int>* zBuffer = nullptr)
{
	auto sortedVertices = sortVertices(points, comparePoints);
	auto vertexChains = splitVertices(sortedVertices, comparePoints);

	auto leftChain = std::get<0>(vertexChains);
	auto currentPointLeft = *(leftChain.begin());
	std::for_each(std::next(leftChain.begin()), leftChain.end(), [&currentPointLeft, drawable, zBuffer](auto point)
	{
		renderLine(Line(currentPointLeft, point), drawable, DDALineRenderer, 1.0, zBuffer);
		currentPointLeft = point;
	});

	auto rightChain = std::get<1>(vertexChains);
	auto currentPointRight = *(rightChain.begin());
	std::for_each(std::next(rightChain.begin()), rightChain.end(), [&currentPointRight, drawable, zBuffer](auto point)
	{
		renderLine(Line(currentPointRight, point), drawable, DDALineRenderer, 1.0, zBuffer);
		currentPointRight = point;
	});
}

void renderPolygonWireframe(const Polygon& polygon, Drawable* drawable, Matrix2D<int>* zBuffer)
{
	auto points = polygon.vertices();
	renderPolygonWireframe(points, drawable, zBuffer);
}
