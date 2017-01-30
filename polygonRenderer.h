#pragma once
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>

#include "drawable.h"
#include "primitives.h"

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
	auto frontPoint = vertices.front();
	std::vector<Point> leftChain = { vertices.front(), vertices.back() };
	std::vector<Point> rightChain = { vertices.front(), vertices.back() };
	auto endIt = std::prev(vertices.end());
	for (auto currentIt = std::next(vertices.begin()); currentIt != endIt; currentIt = std::next(currentIt))
	{
		auto currentPoint = *currentIt;
		if (currentPoint.x <= frontPoint.x)
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

void renderPolygon(const Polygon& polygon, Drawable* drawable)
{
	auto sortedVertices = sortVertices(polygon.vertices(), comparePoints);

	auto vertexChains = splitVertices(sortedVertices, comparePoints);
	auto leftChain = std::get<0>(vertexChains);
	auto rightChain = std::get<1>(vertexChains);

	auto topPoint = sortedVertices.front();
	auto bottomPoint = sortedVertices.back();

	auto leftIter = std::next(leftChain.begin());
	Line leftLine{ leftChain.front(), *leftIter };

	auto rightIter = std::next(rightChain.begin());
	Line rightLine{ rightChain.front(), *rightIter };

	auto ml = getXSlope(leftLine);
	auto mr = getXSlope(rightLine);
	auto xl = static_cast<double>(leftLine.p1.x);
	auto xr = static_cast<double>(rightLine.p1.x);

	for (auto y = topPoint.y; y < bottomPoint.y; ++y, xl += ml, xr += mr)
	{
		for (auto x = xl; x <= xr; ++x)
		{
			auto currentPoint = Point{ static_cast<int>(std::round(x)), y, topPoint.parent };
			auto globalPoint = currentPoint.toGlobalCoordinate();
			drawable->setPixel(globalPoint.x, globalPoint.y, 0xffffffff);
		}

		if (y == leftLine.p2.y)
		{
			leftIter = std::next(leftIter);
			leftLine = Line{ leftLine.p2, *leftIter };
			ml = getXSlope(leftLine);
			xl = static_cast<double>(leftLine.p1.x);
		}

		if (y == rightLine.p2.y)
		{
			rightIter = std::next(rightIter);
			rightLine = Line{ rightLine.p2, *rightIter };
			mr = getXSlope(rightLine);
			xr = static_cast<double>(rightLine.p1.x);
		}
	}
}