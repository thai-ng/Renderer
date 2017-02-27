#pragma once
#include <cmath>
#include <utility>

#include "color.h"
#include "drawable.h"
#include "primitives.h"
#include "lerp.h"

enum class Octant
{
	First,
	Second,
	Third,
	Fourth,
	Fifth,
	Sixth,
	Seventh,
	Eighth
};

Point toFirstOctant(Octant octant, Point p)
{
    switch (octant)
    {
		case Octant::First:
        {
            return Point{p.x, p.y, p.z, p.parent, p.color};
        } break;

        case Octant::Second:
        {
            return Point {p.y, p.x, p.z, p.parent, p.color };
		} break;
		
		case Octant::Third:
		{
			return Point{p.y, -p.x, p.z, p.parent, p.color };
		} break;
		
		case Octant::Fourth:
		{
			return Point{-p.x, p.y, p.z, p.parent, p.color };
		} break;
		
		case Octant::Fifth:
		{
			return Point{-p.x, -p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Sixth:
		{
			return Point{-p.y, -p.x, p.z, p.parent, p.color };
		} break;
		
		case Octant::Seventh:
		{
			return Point{-p.y, p.x, p.z, p.parent, p.color };
		} break;
		
		case Octant::Eighth:
		{
			return Point{p.x, -p.y, p.z, p.parent, p.color };
		} break;

        default:
        {
            throw std::invalid_argument("Invalid octant");
        }
    }
}

Point fromFirstOctant(Octant octant, Point p)
{
	switch (octant)
	{
		case Octant::First:
		{
			return Point{ p.x, p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Second:
		{
			return Point{ p.y, p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Third:
		{
			return Point{ -p.y, p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Fourth:
		{
			return Point{ -p.x, p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Fifth:
		{
			return Point{ -p.x, -p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Sixth:
		{
			return Point{ -p.y, -p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Seventh:
		{
			return Point{ p.y, -p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Eighth:
		{
			return Point{ p.x, -p.y, p.z, p.parent, p.color };
		} break;

        default:
        {
            throw std::invalid_argument("Invalid octant");
        }
	}
}

Octant getOctant(Point p)
{
    auto aboveX = p.y > 0;
    auto rightY = p.x > 0;
	auto aboveDiag = std::abs(p.x) < std::abs(p.y);

	if (aboveX && rightY && !aboveDiag)
	{
		return Octant::First;
	} 
	else if (aboveX && rightY && aboveDiag)
	{
		return Octant::Second;
	}
	else if (aboveX && !rightY && aboveDiag)
	{
		return Octant::Third;
	}
	else if (aboveX && !rightY && !aboveDiag)
	{
		return Octant::Fourth;
	}
	else if (!aboveX && !rightY && !aboveDiag)
	{
		return Octant::Fifth;
	}
	else if (!aboveX && !rightY && aboveDiag)
	{
		return Octant::Sixth;
	}
	else if (!aboveX && rightY && aboveDiag)
	{
		return Octant::Seventh;
	}
	else if (!aboveX && rightY && !aboveDiag)
	{
		return Octant::Eighth;
	}
    else
    {
        throw std::invalid_argument("Invalid octant");
    }
}

bool pointInRect(const Point& point, const Rect& rect)
{
	return point.x >= rect.x && point.x < rect.right() && point.y >= rect.y && point.y < rect.bottom();
}

template <typename F>
void renderLine(const Point& p1, const Point& p2, Drawable* surface, F function, Matrix2D<int>* zBuffer = nullptr)
{
	function(p1, p2, surface, 1.0, zBuffer, p1.parent);
}

template <typename F>
void renderLine(const Line& l, Drawable* surface, F function, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr)
{
	auto points = l.toGlobalCoordinate();
	auto p1 = std::get<0>(points);
	auto p2 = std::get<1>(points);

	function(p1, p2, surface, opacity, zBuffer, p1.parent);
}

auto getColorLerp(const Point& point1, const Point& point2)
{
	auto p1ColorChannels = point1.color.getColorChannels();
	auto p1Red = std::get<0>(p1ColorChannels);
	auto p1Green = std::get<1>(p1ColorChannels);
	auto p1Blue = std::get<2>(p1ColorChannels);

	auto p2ColorChannels = point2.color.getColorChannels();
	auto p2Red = std::get<0>(p2ColorChannels);
	auto p2Green = std::get<1>(p2ColorChannels);
	auto p2Blue = std::get<2>(p2ColorChannels);


	Lerp<decltype(point1.x)> redLerp(point1.x, point2.x, p1Red, p2Red);
	Lerp<decltype(point1.x)> greenLerp(point1.x, point2.x, p1Green, p2Green);
	Lerp<decltype(point1.x)> blueLerp(point1.x, point2.x, p1Blue, p2Blue);


	return std::make_tuple(redLerp, greenLerp, blueLerp);
}

// Draw to surface
void drawToSurface(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint)
{
	drawSurface->setPixel(screenPoint.x, screenPoint.y, colorToPaint.asUnsigned());
}

// Zbuffer draw with no viewport
void drawWithZBuffer(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint, Matrix2D<int>* zBuffer)
{
	if (screenPoint.z <= (*zBuffer)[screenPoint.x][screenPoint.y])
	{
		(*zBuffer)[screenPoint.x][screenPoint.y] = screenPoint.z;
		drawToSurface(screenPoint, drawSurface, colorToPaint);
	}
}

// Zbuffer draw with Viewport
void drawWithZBuffer(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint, Matrix2D<int>* zBuffer, Rect* viewPort)
{
	if (screenPoint.z < (*zBuffer)[screenPoint.x - viewPort->x][screenPoint.y - viewPort->y])
	{
		(*zBuffer)[screenPoint.x - viewPort->x][screenPoint.y - viewPort->y] = screenPoint.z;
		drawToSurface(screenPoint, drawSurface, colorToPaint);
	}
}

// Draw with no viewport
void drawPixel(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint, Matrix2D<int>* zBuffer)
{
	if (zBuffer != nullptr)
	{
		drawWithZBuffer(screenPoint, drawSurface, colorToPaint, zBuffer);
	}
	else
	{
		drawToSurface(screenPoint, drawSurface, colorToPaint);
	}
}

// Draw with Viewport
void drawPixel(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint, Matrix2D<int>* zBuffer, Rect* viewPort)
{
	if (zBuffer != nullptr)
	{
		drawWithZBuffer(screenPoint, drawSurface, colorToPaint, zBuffer, viewPort);
	}
	else
	{
		drawToSurface(screenPoint, drawSurface, colorToPaint);
	}
}

Color getCurrentColor(int x, int y, Drawable* surface)
{
	return Color{ surface->getPixel(x, y) };
}

Color getColorFromLerp(int colorIndex, const std::tuple<Lerp<int>, Lerp<int>, Lerp<int>>& colorLerps)
{
	auto redLerp = std::get<0>(colorLerps);
	auto greenLerp = std::get<1>(colorLerps);
	auto blueLerp = std::get<2>(colorLerps);

	auto r = static_cast<unsigned char>(redLerp[colorIndex].second);
	auto g = static_cast<unsigned char>(greenLerp[colorIndex].second);
	auto b = static_cast<unsigned char>(blueLerp[colorIndex].second);

	return Color{ r, g, b };
}

void BresenhamLineRenderer(const Point& p1, const Point& p2, Drawable* drawSurface, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr, Rect* viewPort = nullptr)
{
	auto octant = getOctant(p2 - p1);
	auto point1 = toFirstOctant(octant, p1);
	auto point2 = toFirstOctant(octant, p2);

	auto dx = point2.x - point1.x;
	auto dy = point2.y - point1.y;
	auto two_dy = 2 * dy;

	auto error = two_dy - dx;
	auto y = point1.y;
	auto screenPoint = fromFirstOctant(octant, point1);
	
	if (point1.color == point2.color)
	{
		for (auto x = point1.x + 1; x <= point2.x; ++x)
		{
			if (error > 0)
			{
				error -= dx;
				y += 1;
			}
			error += dy;
			screenPoint = fromFirstOctant(octant, Point{ x, y });

			auto oldColor = getCurrentColor(screenPoint.x, screenPoint.y, drawSurface);
			auto colorToPaint = colorWithOpacity(point1.color, oldColor, opacity);

			if (viewPort)
			{
				if (pointInRect(screenPoint, *viewPort))
				{
					drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer);
				}
			}
			else
			{
				drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer);
			}
		}
	}
	else
	{
		auto colorLerps = getColorLerp(point1, point2);
		auto colorIndex = 0;

		drawSurface->setPixel(screenPoint.x, screenPoint.y, getColorFromLerp(colorIndex, colorLerps).asUnsigned());
		for (auto x = point1.x + 1; x <= point2.x; ++x)
		{
			++colorIndex;

			if (error > 0)
			{
				error -= dx;
				y += 1;
			}
			error += dy;
			screenPoint = fromFirstOctant(octant, Point{ x, y });

			auto newColor = getColorFromLerp(colorIndex, colorLerps);
			auto oldColor = drawSurface->getPixel(screenPoint.x, screenPoint.y);
			auto colorToPaint = colorWithOpacity(newColor, oldColor, opacity);

			if (viewPort)
			{
				if (pointInRect(screenPoint, *viewPort))
				{
					drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer, viewPort);
				}
			}
			else
			{
				drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer);
			}
		}
	}
	
}

void DDALineRenderer(const Point& p1, const Point& p2, Drawable* drawSurface, double opacity = 1.0, Matrix2D<int>* zBuffer = nullptr, Rect* viewPort = nullptr)
{
	auto octant = getOctant(p2 - p1);
	auto point1 = toFirstOctant(octant, p1);
	auto point2 = toFirstOctant(octant, p2);

	if (point1.color == point2.color)
	{
		Lerp<decltype(point1.x)> posLerp(point1.x, point2.x, point1.y, point2.y);
		Lerp<decltype(point1.x)> zLerp(point1.x, point2.x, point1.z, point2.z);

		for (auto i = 0u; i < posLerp.size(); ++i)
		{
			auto point = posLerp[i];
			auto x = point.first;
			auto y = point.second;
			auto z = zLerp[i].second;
			auto screenPoint = fromFirstOctant(octant, Point{ x, static_cast<int>(std::round(y)), static_cast<int>(std::round(z)) });

			auto oldColor = getCurrentColor(screenPoint.x, screenPoint.y, drawSurface);
			auto colorToPaint = colorWithOpacity(point1.color, oldColor, opacity);

			if (viewPort)
			{
				if (pointInRect(screenPoint, *viewPort))
				{
					drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer, viewPort);
				}
			}
			else
			{
				drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer);
			}
		}
	}
	else
	{
		auto colorLerps = getColorLerp(point1, point2);
		Lerp<decltype(point1.x)> posLerp(point1.x, point2.x, point1.y, point2.y);
		Lerp<decltype(point1.x)> zLerp(point1.x, point2.x, point1.z, point2.z);

		for (auto i = 0u; i < posLerp.size(); ++i)
		{
			auto point = posLerp[i];
			auto x = point.first;
			auto y = point.second;
			auto z = zLerp[i].second;

			auto screenPoint = fromFirstOctant(octant, Point{ x, static_cast<int>(std::round(y)), static_cast<int>(std::round(z)) });

			auto newColor = getColorFromLerp(i, colorLerps);
			auto oldColor = getCurrentColor(screenPoint.x, screenPoint.y, drawSurface);
			auto colorToPaint = colorWithOpacity(newColor, oldColor, opacity);

			if (viewPort)
			{
				if (pointInRect(screenPoint, *viewPort))
				{
					drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer, viewPort);
				}
			}
			else
			{
				drawPixel(screenPoint, drawSurface, colorToPaint, zBuffer);
			}
		}
	}
}