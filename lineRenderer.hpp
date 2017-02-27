#pragma once

#include "color.hpp"
#include "drawable.h"
#include "lerp.hpp"
#include "primitives.hpp"

void BresenhamLineRenderer(const Point& p1, const Point& p2, Drawable* drawSurface, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr, const Rect* viewPort = nullptr);

void DDALineRenderer(const Point& p1, const Point& p2, Drawable* drawSurface, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr, const Rect* viewPort = nullptr);

template <typename F>
void renderLine(const Point& p1, const Point& p2, Drawable* surface, F function, Matrix2D<int>* zBuffer = nullptr)
{
	function(p1, p2, surface, 1.0, zBuffer, p1.parent);
}

template <typename F>
void renderLine(const Point& p1, const Point& p2, Drawable* surface, F function, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr, Rect* viewPort = nullptr)
{
	function(p1, p2, surface, opacity, zBuffer, viewPort);
}

template <typename F>
void renderLine(const Line& l, Drawable* surface, F function, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr)
{
	auto points = l.toGlobalCoordinate();
	auto p1 = std::get<0>(points);
	auto p2 = std::get<1>(points);

	function(p1, p2, surface, opacity, zBuffer, p1.parent);
}

std::tuple<Lerp<int>, Lerp<int>, Lerp<int>> getColorLerp(const Point& point1, const Point& point2);