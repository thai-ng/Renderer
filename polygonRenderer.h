#pragma once
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>

#include "color.h"
#include "drawable.h"
#include "primitives.h"
#include "lineRenderer.h"

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

template <typename T, typename F>
T sortVertices(const T& vertices, F comp)
{
    T sortedVertices{vertices};
	std::sort(sortedVertices.begin(), sortedVertices.end(), comp);
	return sortedVertices;
}

template<typename T, typename F>
std::tuple<T, T> splitVertices(const T& vertices, F sortComp)
{
	std::vector<Point> leftChain = { vertices.front(), vertices.back() };
	std::vector<Point> rightChain = { vertices.front(), vertices.back() };
	
	auto centerX = 0;
	for (auto point : vertices)
	{
		centerX += point.x;
	}
	centerX /= static_cast<int>(vertices.size());

	auto endIt = std::prev(vertices.end());
	for (auto currentIt = std::next(vertices.begin()); currentIt != endIt; currentIt = std::next(currentIt))
	{
		auto currentPoint = *currentIt;
		if (currentPoint.x <= centerX)
		{
			leftChain.push_back(currentPoint);
		}
		else
		{
			rightChain.push_back(currentPoint);
		}
	}

	std::sort(leftChain.begin(), leftChain.end(), sortComp);
	std::sort(rightChain.begin(), rightChain.end(), sortComp);
	return std::make_tuple(leftChain, rightChain);
}


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


void renderPolygon(const std::vector<Point>& points, Drawable* drawable, double opacity = 1.0)
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

	auto rightColorLerps = getColorLerp(rightLine.p1.flipped(), rightLine.p2.flipped());
	auto rightRedLerp = std::get<0>(rightColorLerps);
	auto rightGreenLerp = std::get<1>(rightColorLerps);
	auto rightBlueLerp = std::get<2>(rightColorLerps);

	auto rightCount = 0;

	for (auto y = topPoint.y; y <= bottomPoint.y; ++y)
	{
		auto xl = leftLerp[leftCount].second;
		auto r = static_cast<unsigned char>(leftRedLerp[leftCount].second);
		auto g = static_cast<unsigned char>(leftGreenLerp[leftCount].second);
		auto b = static_cast<unsigned char>(leftBlueLerp[leftCount].second);
		++leftCount;
		auto leftColor = Color(r, g, b);

		auto xr = rightLerp[rightCount].second;
		r = static_cast<unsigned char>(rightRedLerp[rightCount].second);
		g = static_cast<unsigned char>(rightGreenLerp[rightCount].second);
		b = static_cast<unsigned char>(rightBlueLerp[rightCount].second);
		++rightCount;
		auto rightColor = Color(r, g, b);

		auto leftPoint = Point{ static_cast<int>(std::round(xl)), y, topPoint.parent, leftColor };
		auto rightPoint = Point{ static_cast<int>(std::round(xr)), y, topPoint.parent, rightColor };

		renderLine(Line{ leftPoint, rightPoint }, drawable, DDALineRenderer, opacity);

		if (y == leftLine.p2.y && leftLine.p2 != bottomPoint)
		{
			leftIter = std::next(leftIter);
			leftLine = Line{ leftLine.p2, *leftIter };
			leftLerp = Lerp<int>(leftLine.p1.y, leftLine.p2.y, leftLine.p1.x, leftLine.p2.x);
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
			rightColorLerps = getColorLerp(rightLine.p1.flipped(), rightLine.p2.flipped());
			rightRedLerp = std::get<0>(rightColorLerps);
			rightGreenLerp = std::get<1>(rightColorLerps);
			rightBlueLerp = std::get<2>(rightColorLerps);
			rightCount = 0;
		}
	}
}

void renderPolygon(const std::vector<Point>& points, Drawable* drawable, const Color& color, double opacity = 1.0)
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

		auto leftPoint = Point{ static_cast<int>(std::round(xl)), y, topPoint.parent, color };
		auto rightPoint = Point{ static_cast<int>(std::round(xr)), y, topPoint.parent, color };

		renderLine(Line{ leftPoint, rightPoint }, drawable, DDALineRenderer, opacity);

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

void renderPolygon(const Polygon& polygon, Drawable* drawable, const Color& color)
{
	renderPolygon(polygon.vertices(), drawable, color);
}

void renderTriangle(const Triangle& triangle, Drawable* drawable, const Color& color, double opacity = 1.0)
{
	auto vertices = triangle.vertices();
	renderPolygon(std::vector<Point>(vertices.begin(), vertices.end()), drawable, color, opacity);
}

void renderTriangle(const Triangle& triangle, Drawable* drawable, double opacity = 1.0)
{
	auto vertices = triangle.vertices();
	renderPolygon(std::vector<Point>(vertices.begin(), vertices.end()), drawable, opacity);
}


void renderPolygonWireframe(const Polygon& polygon, Drawable* drawable)
{
	auto points = polygon.vertices();
	auto sortedVertices = sortVertices(points, comparePoints);
	auto vertexChains = splitVertices(sortedVertices, comparePoints);
	
	auto leftChain = std::get<0>(vertexChains);
	auto currentPointLeft = *(leftChain.begin());
	std::for_each(std::next(leftChain.begin()), leftChain.end(), [&currentPointLeft, drawable](auto point) 
	{ 
		renderLine(Line(currentPointLeft, point), drawable, DDALineRenderer); 
		currentPointLeft = point;
	});
	
	auto rightChain = std::get<1>(vertexChains);
	auto currentPointRight = *(rightChain.begin());
	std::for_each(std::next(rightChain.begin()), rightChain.end(), [&currentPointRight, drawable](auto point)
	{
		renderLine(Line(currentPointRight, point), drawable, DDALineRenderer);
		currentPointRight = point;
	});
}
