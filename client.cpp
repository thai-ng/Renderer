#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <random>

#include "client.h"
#include "primitives.h"
#include "lineRenderer.h"
#include "polygonRenderer.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}

std::vector<Line> generateStarburstLines(int number, int length, Rect& parent)
{
	std::vector<Line> lines;
	int degree = 0;
	std::generate_n(std::back_inserter(lines), number, [&degree, &parent , number, length]()
	{
        auto line = Line{ parent.center(), length, degree, &parent };
		degree += static_cast<int>(360.0 / number);
		return line;
	});

    return lines;
}

template <typename F>
void drawStarburst(Rect& panel, Drawable* drawable, int numberOfLines, int length, F renderAlgorithm1, F renderAlgorithm2)
{
	auto lines = generateStarburstLines(numberOfLines, length, panel);

	bool algorithmSwitch = true;
    std::for_each(lines.begin(), lines.end(), [&algorithmSwitch, drawable, renderAlgorithm1, renderAlgorithm2](auto line)
	{
		if (algorithmSwitch)
		{
			renderLine(line, drawable, renderAlgorithm1);
		}
		else
		{
			renderLine(line, drawable, renderAlgorithm2);
		}

		algorithmSwitch = !algorithmSwitch;
	});
}

template <typename F>
void drawStarburst(Rect& panel, Drawable* drawable, int numberOfLines, int length, F renderAlgorithm)
{
	auto lines = generateStarburstLines(numberOfLines, length, panel);

	std::for_each(lines.begin(), lines.end(), [drawable, renderAlgorithm](auto line)
	{
		renderLine(line, drawable, renderAlgorithm);
	});
}

template <typename F>
void drawParallelogram(Rect& panel, Drawable* drawable, F renderAlgorithm1, F renderAlgorithm2)
{
	bool algorithmSwitch = true;
	for (auto p = 0; p <= 50; ++p)
	{
		if (algorithmSwitch)
		{
			auto line1 = Line{ Point{ 20, 80 + p, &panel }, Point{ 150, 150 + p, &panel } };
			renderLine(line1, drawable, renderAlgorithm1);

			auto line2 = Line{ Point{ 160 + p, 270, &panel }, Point{ 240 + p, 40, &panel } };
			renderLine(line2, drawable, renderAlgorithm1);
		}
		else
		{
			auto line1 = Line{ Point{ 20, 80 + p, &panel }, Point{ 150, 150 + p, &panel } };
			renderLine(line1, drawable, renderAlgorithm2);

			auto line2 = Line{ Point{ 160 + p, 270, &panel }, Point{ 240 + p, 40, &panel } };
			renderLine(line2, drawable, renderAlgorithm2);
		}
	}
}

template <typename F>
void drawParallelogram(Rect& panel, Drawable* drawable, F renderAlgorithm)
{
	for (auto p = 0; p <= 50; ++p)
	{
		auto line1 = Line{ Point{ 20, 80 + p, &panel }, Point{ 150, 150 + p, &panel } };
		renderLine(line1, drawable, renderAlgorithm);

		auto line2 = Line{ Point{ 160 + p, 270, &panel }, Point{ 240 + p, 40, &panel } };
		renderLine(line2, drawable, renderAlgorithm);
	}
}

template <typename F>
void drawRandomLines(Rect& panel, Drawable* drawable, const std::vector<Line>& lines, F renderAlgorithm)
{
	std::for_each(lines.begin(), lines.end(), [&panel, drawable, renderAlgorithm](auto line)
	{
		line.setParent(&panel);
		renderLine(line, drawable, renderAlgorithm);
	});
}

template <typename F>
void drawRandomLines(Rect& panel, Drawable* drawable, const std::vector<Line>& lines, F renderAlgorithm1, F renderAlgorithm2)
{
	bool algorithmSwitch = true;
	std::for_each(lines.begin(), lines.end(), [&algorithmSwitch, &panel, drawable, renderAlgorithm1, renderAlgorithm2](auto line)
	{
		line.setParent(&panel);
		if (algorithmSwitch)
		{
			renderLine(line, drawable, renderAlgorithm1);
		}
		else
		{
			renderLine(line, drawable, renderAlgorithm2);
		}
		algorithmSwitch = !algorithmSwitch;
	});
}

auto generateRandomLines()
{
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<> coordinateDis(0, 299);
	std::uniform_int_distribution<> colorDis(0x00000000, 0x00ffffff);

	std::vector<Line> lines;
	lines.reserve(30);
	std::generate_n(std::back_inserter(lines), 30, [&colorDis, &coordinateDis, &gen]
	{
		auto color = 0xff000000u + static_cast<unsigned int>(colorDis(gen));
		return Line{ Point{ coordinateDis(gen), coordinateDis(gen)}, Point{ coordinateDis(gen), coordinateDis(gen)}, static_cast<unsigned int>(color) };
	});
	return lines;
}

void Client::nextPage() {
    static int pageNumber = 0;
    pageNumber++;
    std::cout << "PageNumber " << pageNumber << std::endl;
	
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


    switch(pageNumber % 5) 
	{
		case 1:
		{
			drawStarburst(panel1, drawable, 90, 125, DDALineRenderer);
			drawStarburst(panel2, drawable, 90, 125, BresenhamLineRenderer);
			drawStarburst(panel3, drawable, 90, 125, DDALineRenderer, BresenhamLineRenderer);

		} break;

		case 2:
		{
			drawParallelogram(panel1, drawable, DDALineRenderer);
			drawParallelogram(panel2, drawable, BresenhamLineRenderer);
			drawParallelogram(panel3, drawable, DDALineRenderer, BresenhamLineRenderer);
		} break;
		case 3:
		{
			auto lines = generateRandomLines();
			drawRandomLines(panel1, drawable, lines, DDALineRenderer);
			drawRandomLines(panel2, drawable, lines, BresenhamLineRenderer);
			drawRandomLines(panel3, drawable, lines, DDALineRenderer, BresenhamLineRenderer);

		} break;
		case 4:
		{
			Polygon polygon{ std::vector<Point>{Point{ 50, 10 }, Point{ 10, 50 }, Point{ 90, 50 }, Point{ 60, 100 }}, &panel1 };
			renderPolygon(polygon, drawable);
		} break;
		// fall through...
		default:
		{
			draw_rect(0, 0, 750, 750, 0xffffffff);
			draw_rect(400, 400, 700, 700, 0xff00ff40);
		} break;
	}

	drawable->updateScreen();   // you must call this to make the display change.

}

void Client::draw_rect(int x1, int y1, int x2, int y2, unsigned int color) {
    for(int x = x1; x<x2; x++) 
	{
        for(int y=y1; y<y2; y++) 
		{
            drawable->setPixel(x, y, color);
        }
    }
}
