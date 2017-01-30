#include <cmath>
#include <utility>
#include "drawable.h"
#include "primitives.h"

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
	auto originalOctant = getOctant(p2 - p1);
    auto converted_p1 = toFirstOctant(originalOctant, p1);
    auto converted_p2 = toFirstOctant(originalOctant, p2);
    function(converted_p1, converted_p2, originalOctant, surface, color);
}

template <typename F>
void renderLine(const Line& l, Drawable* surface, F function)
{
	auto points = l.toGlobalCoordinate();
	auto p1 = std::get<0>(points);
	auto p2 = std::get<1>(points);

	auto originalOctant = getOctant(p2 - p1);
	auto converted_p1 = toFirstOctant(originalOctant, p1);
	auto converted_p2 = toFirstOctant(originalOctant, p2);
	function(converted_p1, converted_p2, originalOctant, surface, l.color);
}


void BresenhamLineRenderer(Point p1, Point p2, Octant octant, Drawable* drawSurface, unsigned int color)
{
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

void DDALineRenderer(Point p1, Point p2, Octant octant, Drawable* drawSurface, unsigned int color)
{
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
