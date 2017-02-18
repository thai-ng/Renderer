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
            return Point{p.x, p.y};
        } break;

        case Octant::Second:
        {
            return Point {p.y, p.x};
		} break;
		
		case Octant::Third:
		{
			return Point{p.y, -p.x};
		} break;
		
		case Octant::Fourth:
		{
			return Point{-p.x, p.y};
		} break;
		
		case Octant::Fifth:
		{
			return Point{-p.x, -p.y};
		} break;

		case Octant::Sixth:
		{
			return Point{-p.y, -p.x};
		} break;
		
		case Octant::Seventh:
		{
			return Point{-p.y, p.x};
		} break;
		
		case Octant::Eighth:
		{
			return Point{p.x, -p.y};
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
			return Point{ p.x, p.y };
		} break;

		case Octant::Second:
		{
			return Point{ p.y, p.x };
		} break;

		case Octant::Third:
		{
			return Point{ -p.y, p.x };
		} break;

		case Octant::Fourth:
		{
			return Point{ -p.x, p.y };
		} break;

		case Octant::Fifth:
		{
			return Point{ -p.x, -p.y };
		} break;

		case Octant::Sixth:
		{
			return Point{ -p.y, -p.x };
		} break;

		case Octant::Seventh:
		{
			return Point{ p.y, -p.x };
		} break;

		case Octant::Eighth:
		{
			return Point{ p.x, -p.y };
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

template <typename F>
void renderLine(const Point& p1, const Point& p2, Drawable* surface, const Color& color, F function)
{
	function(p1, p2, surface, color);
}

template <typename F>
void renderLine(const Line& l, Drawable* surface, F function)
{
	auto points = l.toGlobalCoordinate();
	auto p1 = std::get<0>(points);
	auto p2 = std::get<1>(points);

	function(p1, p2, surface, l.color);
}

void BresenhamLineRenderer(const Point& p1, const Point& p2, Drawable* drawSurface, const Color& color)
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
	drawSurface->setPixel(screenPoint.x, screenPoint.y, color.asUnsigned());
    for (auto x = point1.x + 1; x <= point2.x; ++x)
	{
		if (error > 0)
		{
			error -= dx;
            y += 1;
		}
		error += dy;
		screenPoint = fromFirstOctant(octant, Point{ x, y });
		drawSurface->setPixel(screenPoint.x, screenPoint.y, color.asUnsigned());
	}
}

void DDALineRenderer(const Point& p1, const Point& p2, Drawable* drawSurface, const Color& color)
{
	auto octant = getOctant(p2 - p1);
	auto point1 = toFirstOctant(octant, p1);
	auto point2 = toFirstOctant(octant, p2);

	Lerp<decltype(point1.x)> lerp(point1.x, point2.x, point1.y, point2.y);
	for (auto& point : lerp)
	{
		auto x = point.first;
		auto y = point.second;

		auto screenPoint = fromFirstOctant(octant, Point{ x, static_cast<int>(std::round(y)) });
		drawSurface->setPixel(screenPoint.x, screenPoint.y, color.asUnsigned());
	}
}

double fpart(double num)
{
	if (num < 0)
	{
		return 1 - (num - std::floor(num));
	}
	else
	{
		return num - std::floor(num);
	}
}

double rfpart(double num)
{
	return 1 - fpart(num);
}

void WuLineRenderer(const Point& p1, const Point& p2, Drawable* drawable, const Color& color)
{
	auto point1 = p1;
	auto point2 = p2;
	bool steep = std::abs(point2.y - point1.y) > std::abs(point2.x - point1.x);
	if (steep)
	{
		std::swap(point1.x, point1.y);
		std::swap(point2.x, point2.y);
	}

	if (point1.x > point2.x)
	{
		std::swap(point1.x, point2.x);
		std::swap(point1.y, point2.y);
	}

	auto dx = point2.x - point1.x;
	auto dy = point2.y - point1.y;
	auto gradient = static_cast<double>(dy) / static_cast<double>(dx);
	if (dx == 0.0)
	{
		gradient = 1.0;
	}

    double xend = point1.x;
    double yend = point1.y;
	auto xgap = rfpart(point1.x + 0.5);
    double xpixel1 = xend;
    double ypixel1 = yend;

	if (steep)
	{
		auto currentColor = drawable->getPixel(static_cast<int>(ypixel1), static_cast<int>(xpixel1));
		
		auto colorToDraw = colorWithOpacity(color, currentColor, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel1), static_cast<int>(xpixel1), colorToDraw.asUnsigned());

		currentColor = drawable->getPixel(static_cast<int>(ypixel1) + 1, static_cast<int>(xpixel1));
		colorToDraw = colorWithOpacity(color, currentColor, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel1) + 1, static_cast<int>(xpixel1), colorToDraw.asUnsigned());
	}
	else
	{
		auto currentColor = drawable->getPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1));
		auto colorToDraw = colorWithOpacity(color, currentColor, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1), colorToDraw.asUnsigned());

		currentColor = drawable->getPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1) + 1);
		colorToDraw = colorWithOpacity(color, currentColor, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1) + 1, colorToDraw.asUnsigned());
	}

	auto yIntersection = yend + gradient;

	xend = point2.x;
	yend = point2.y;
	xgap = rfpart(point2.x + 0.5);
    double xpixel2 = xend;
    double ypixel2 = yend;

	if (steep)
	{
		auto currentColor = drawable->getPixel(static_cast<int>(ypixel2), static_cast<int>(xpixel2));
		auto colorToDraw = colorWithOpacity(color, currentColor, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel2), static_cast<int>(xpixel2), colorToDraw.asUnsigned());

		currentColor = drawable->getPixel(static_cast<int>(ypixel2) + 1, static_cast<int>(xpixel2));
		colorToDraw = colorWithOpacity(color, currentColor, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel2) + 1, static_cast<int>(xpixel2), colorToDraw.asUnsigned());
	}
	else
	{
		auto currentColor = drawable->getPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2));
		auto colorToDraw = colorWithOpacity(color, currentColor, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2), colorToDraw.asUnsigned());

		currentColor = drawable->getPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2) + 1);
		colorToDraw = colorWithOpacity(color, currentColor, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2) + 1, colorToDraw.asUnsigned());
	}

	if (steep)
	{
		for (auto x = static_cast<int>(xpixel1) + 1; x < static_cast<int>(xpixel2); ++x, yIntersection += gradient)
		{
			auto currentColor = drawable->getPixel(static_cast<int>(yIntersection), x);
			auto colorToDraw = colorWithOpacity(color, currentColor, rfpart(yIntersection));
			drawable->setPixel(static_cast<int>(yIntersection), x, colorToDraw.asUnsigned());

			currentColor = drawable->getPixel(static_cast<int>(yIntersection) + 1, x);
			colorToDraw = colorWithOpacity(color, currentColor, fpart(yIntersection));
			drawable->setPixel(static_cast<int>(yIntersection) + 1, x, colorToDraw.asUnsigned());
		}
	}
	else
	{
		for (auto x = static_cast<int>(xpixel1) + 1; x < static_cast<int>(xpixel2); ++x, yIntersection += gradient)
		{
			auto currentColor = drawable->getPixel(x, static_cast<int>(yIntersection));
			auto colorToDraw = colorWithOpacity(color, currentColor, rfpart(yIntersection));
			drawable->setPixel(x, static_cast<int>(yIntersection), colorToDraw.asUnsigned());

			currentColor = drawable->getPixel(x, static_cast<int>(yIntersection) + 1);
			colorToDraw = colorWithOpacity(color, currentColor, fpart(yIntersection));
			drawable->setPixel(x, static_cast<int>(yIntersection) + 1, colorToDraw.asUnsigned());
		}
	}
}
