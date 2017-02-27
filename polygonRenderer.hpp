#pragma once
#include <vector>
#include <tuple>
#include <algorithm>

#include "color.hpp"
#include "drawable.h"
#include "primitives.hpp"

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

void renderPolygon(const Polygon& polygon, Drawable* drawable, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr);

void renderPolygon(const Polygon& polygon, Drawable* drawable, const Color& color, Matrix2D<int>* zBuffer = nullptr);

void renderTriangle(const Triangle& triangle, Drawable* drawable, const Color& color, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr);

void renderTriangle(const Triangle& triangle, Drawable* drawable, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr);

void renderPolygonWireframe(const Polygon& polygon, Drawable* drawable, Matrix2D<int>* zBuffer = nullptr);