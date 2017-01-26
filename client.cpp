#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "client.h"
#include "lineRenderer.h"
#include "rect.h"
#include "line.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}

template <typename F>
void doDrawing(Rect& panel, Drawable* drawable, F renderAlgorithm)
{
	Line l1{ Point{ 10, 20, &panel }, Point{ 20, 150, &panel }};

	renderLine(l1, drawable, 0xffffffff, renderAlgorithm);
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

void Client::nextPage() {
    static int pageNumber = 0;
    pageNumber++;
    std::cout << "PageNumber " << pageNumber << std::endl;

    switch(pageNumber % 4) {
    case 1:
	{
		Rect bg{ 0, 0, 750, 750 };
        draw_rect(bg.x, bg.y, bg.right(), bg.bottom(), 0xffffffff);

		Rect panel1{ 50, 50, 300, 300 };
        draw_rect(panel1.x, panel1.y, panel1.right(), panel1.bottom(), 0xff000000);

		Rect panel2{ 400, 50, 300, 300 };
		draw_rect(panel2.x, panel2.y, panel2.right(), panel2.bottom(), 0xff000000);

		Rect panel3{ 50, 400, 300, 300 };
		draw_rect(panel3.x, panel3.y, panel3.right(), panel3.bottom(), 0xff000000);

		Rect panel4{ 400, 400, 300, 300 };
		draw_rect(panel4.x, panel4.y, panel4.right(), panel4.bottom(), 0xff000000);

		doDrawing(panel1, drawable, DDALineRenderer);
		doDrawing(panel2, drawable, BresenhamLineRenderer);

	} break;
    case 2:
	{
		Rect bg{ 0, 0, 750, 750 };
		draw_rect(bg.x, bg.y, bg.right(), bg.bottom(), 0xffffffff);

		Rect panel1{ 50, 50, 300, 300 };
		draw_rect(panel1.x, panel1.y, panel1.right(), panel1.bottom(), 0xff000000);

		Rect panel2{ 400, 50, 300, 300 };
		draw_rect(panel2.x, panel2.y, panel2.right(), panel2.bottom(), 0xff000000);

		Rect panel3{ 50, 400, 300, 300 };
		draw_rect(panel3.x, panel3.y, panel3.right(), panel3.bottom(), 0xff000000);

		Rect panel4{ 400, 400, 300, 300 };
		draw_rect(panel4.x, panel4.y, panel4.right(), panel4.bottom(), 0xff000000);

		Point p1{ 50, 10, &panel1 };
		Point p2{ 10, 50, &panel1 };
		Point p3{ 90, 50, &panel1 };
		Point p4{ 60, 100, &panel1 };
		std::vector<Point> points = { p1, p2, p3, p4 };
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

		// sort vertices
		std::sort(points.begin(), points.end(), comparePoints);

		// Split to left right
		auto frontPoint = points.front();
		std::vector<Point> leftChain = { points.front(), points.back() };
		std::vector<Point> rightChain = { points.front(), points.back() };
		auto topPoint = points.front();
		auto endIt = std::prev(points.end());
		for (auto currentIt = std::next(points.begin()); currentIt != endIt; currentIt = std::next(currentIt))
		{
			auto currentPoint = *currentIt;
			if (currentPoint.x <= topPoint.x)
			{
				leftChain.push_back(currentPoint);
			}
			else
			{
				rightChain.push_back(currentPoint);
			}
		}

		std::sort(leftChain.begin(), leftChain.end(), comparePoints);
		std::sort(rightChain.begin(), rightChain.end(), comparePoints);
		auto bottomPoint = points.back();

		auto secondPointLeftIter = std::next(leftChain.begin());
		Line leftLine{ leftChain.front(), *secondPointLeftIter };

		auto secondPointRightIter = std::next(rightChain.begin());
		Line rightLine{ rightChain.front(), *secondPointRightIter };

		auto ml = getXSlope(leftLine);
		auto mr = getXSlope(rightLine);
		auto xl = static_cast<double>(leftLine.p1.x);
		auto xr = static_cast<double>(rightLine.p1.x);
		for (auto y = topPoint.y; y < bottomPoint.y; ++y)
		{
			for (auto x = xl; x <= xr; ++x)
			{
				auto currentPoint = Point{ static_cast<int>(std::round(x)), y, topPoint.parent };
				auto globalPoint = currentPoint.toGlobalCoordinate();
				drawable->setPixel(globalPoint.x, globalPoint.y, 0xffffffff);
			}

			if (y == leftLine.p2.y)
			{
				secondPointLeftIter = std::next(secondPointLeftIter);
				leftLine = Line{ leftLine.p2, *secondPointLeftIter };
				ml = getXSlope(leftLine);
				xl = static_cast<double>(leftLine.p1.x);
			}

			if (y == rightLine.p2.y)
			{
				secondPointRightIter = std::next(secondPointRightIter);
				rightLine = Line{ rightLine.p2, *secondPointRightIter };
				mr = getXSlope(rightLine);
				xr = static_cast<double>(rightLine.p1.x);
			}

			xl += ml;
			xr += mr;
		}

		for (auto p : points)
		{
			auto globalPoint = p.toGlobalCoordinate();
			drawable->setPixel(globalPoint.x, globalPoint.y, 0xffff0000);
		}
	} break;
    case 3:
        break;
    case 4:
        // fall through...
    default:
        draw_rect(0, 0, 750, 750, 0xffffffff);
        draw_rect(400, 400, 700, 700, 0xff00ff40);
    }

	drawable->updateScreen();   // you must call this to make the display change.

}

void Client::draw_rect(int x1, int y1, int x2, int y2, unsigned int color) {
    for(int x = x1; x<x2; x++) {
        for(int y=y1; y<y2; y++) {
            drawable->setPixel(x, y, color);
        }
    }
}
