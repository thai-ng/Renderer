#include <cmath>
#include <utility>
#include "drawable.h"
#include "primitives.h"
#include "colorUtils.h"

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
void renderLine(Point p1, Point p2, Drawable* surface, unsigned int color, F function)
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


void BresenhamLineRenderer(Point p1, Point p2, Drawable* drawSurface, unsigned int color)
{
	auto octant = getOctant(p2 - p1);
	p1 = toFirstOctant(octant, p1);
	p2 = toFirstOctant(octant, p2);

	auto dx = p2.x - p1.x;
	auto dy = p2.y - p1.y;
	auto two_dy = 2 * dy;

	auto error = two_dy - dx;
	auto y = p1.y;
	auto screenPoint = fromFirstOctant(octant, p1);
	drawSurface->setPixel(screenPoint.x, screenPoint.y, color);
    for (auto x = p1.x + 1; x <= p2.x; ++x)
	{
		if (error > 0)
		{
			error -= dx;
            y += 1;
		}
		error += dy;
		screenPoint = fromFirstOctant(octant, Point{ x, y });
		drawSurface->setPixel(screenPoint.x, screenPoint.y, color);
	}
}

void DDALineRenderer(Point p1, Point p2, Drawable* drawSurface, unsigned int color)
{
	auto octant = getOctant(p2 - p1);
	p1 = toFirstOctant(octant, p1);
	p2 = toFirstOctant(octant, p2);

	auto delta_x = p2.x - p1.x;
	auto delta_y = p2.y - p1.y;
    auto m = static_cast<double>(delta_y) / static_cast<double>(delta_x);
	auto y = static_cast<double>(p1.y);
    for (auto x = p1.x; x <= p2.x; ++x)
	{
        auto screenPoint = fromFirstOctant(octant, Point{x, static_cast<int>(std::round(y))});
        drawSurface->setPixel(screenPoint.x, screenPoint.y, color);
		y += m;
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

void WuLineRenderer(Point p1, Point p2, Drawable* drawable, unsigned int color)
{
	bool steep = std::abs(p2.y - p1.y) > std::abs(p2.x - p1.x);
	if (steep)
	{
		std::swap(p1.x, p1.y);
		std::swap(p2.x, p2.y);
	}

	if (p1.x > p2.x)
	{
		std::swap(p1.x, p2.x);
		std::swap(p1.y, p2.y);
	}

	auto dx = p2.x - p1.x;
	auto dy = p2.y - p1.y;
	auto gradient = static_cast<double>(dy) / static_cast<double>(dx);
	if (dx == 0.0)
	{
		gradient = 1.0;
	}

    double xend = p1.x;
    double yend = p1.y;
	auto xgap = rfpart(p1.x + 0.5);
    double xpixel1 = xend;
    double ypixel1 = yend;

	if (steep)
	{
		auto currentColor = drawable->getPixel(static_cast<int>(ypixel1), static_cast<int>(xpixel1));
		auto colorToDraw = getColorWithOpacity(currentColor, color, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel1), static_cast<int>(xpixel1), colorToDraw);

		currentColor = drawable->getPixel(static_cast<int>(ypixel1) + 1, static_cast<int>(xpixel1));
		colorToDraw = getColorWithOpacity(currentColor, color, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel1) + 1, static_cast<int>(xpixel1), colorToDraw);
	}
	else
	{
		auto currentColor = drawable->getPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1));
		auto colorToDraw = getColorWithOpacity(currentColor, color, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1), colorToDraw);

		currentColor = drawable->getPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1) + 1);
		colorToDraw = getColorWithOpacity(currentColor, color, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel1), static_cast<int>(ypixel1) + 1, colorToDraw);
	}

	auto yIntersection = yend + gradient;

	xend = p2.x;
	yend = p2.y;
	xgap = rfpart(p2.x + 0.5);
    double xpixel2 = xend;
    double ypixel2 = yend;

	if (steep)
	{
		auto currentColor = drawable->getPixel(static_cast<int>(ypixel2), static_cast<int>(xpixel2));
		auto colorToDraw = getColorWithOpacity(currentColor, color, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel2), static_cast<int>(xpixel2), colorToDraw);

		currentColor = drawable->getPixel(static_cast<int>(ypixel2) + 1, static_cast<int>(xpixel2));
		colorToDraw = getColorWithOpacity(currentColor, color, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(ypixel2) + 1, static_cast<int>(xpixel2), colorToDraw);
	}
	else
	{
		auto currentColor = drawable->getPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2));
		auto colorToDraw = getColorWithOpacity(currentColor, color, rfpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2), colorToDraw);

		currentColor = drawable->getPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2) + 1);
		colorToDraw = getColorWithOpacity(currentColor, color, fpart(yend) * xgap);
		drawable->setPixel(static_cast<int>(xpixel2), static_cast<int>(ypixel2) + 1, colorToDraw);
	}

	if (steep)
	{
		for (auto x = static_cast<int>(xpixel1) + 1; x < static_cast<int>(xpixel2); ++x, yIntersection += gradient)
		{
			auto currentColor = drawable->getPixel(static_cast<int>(yIntersection), x);
			auto colorToDraw = getColorWithOpacity(currentColor, color, rfpart(yIntersection));
			drawable->setPixel(static_cast<int>(yIntersection), x, colorToDraw);

			currentColor = drawable->getPixel(static_cast<int>(yIntersection) + 1, x);
			colorToDraw = getColorWithOpacity(currentColor, color, fpart(yIntersection));
			drawable->setPixel(static_cast<int>(yIntersection) + 1, x, colorToDraw);
		}
	}
	else
	{
		for (auto x = static_cast<int>(xpixel1) + 1; x < static_cast<int>(xpixel2); ++x, yIntersection += gradient)
		{
			auto currentColor = drawable->getPixel(x, static_cast<int>(yIntersection));
			auto colorToDraw = getColorWithOpacity(currentColor, color, rfpart(yIntersection));
			drawable->setPixel(x, static_cast<int>(yIntersection), colorToDraw);

			currentColor = drawable->getPixel(x, static_cast<int>(yIntersection) + 1);
			colorToDraw = getColorWithOpacity(currentColor, color, fpart(yIntersection));
			drawable->setPixel(x, static_cast<int>(yIntersection) + 1, colorToDraw);
		}
	}
}
