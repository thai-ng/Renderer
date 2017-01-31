#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <random>
#include <fstream>
#include <numeric>

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

void drawCircularTriangles(Rect& panel, Drawable* drawable, double opacity = 1.0)
{
	auto lines = generateStarburstLines(90, 125, panel);
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<> colorDis(0x00000000, 0x00ffffff);
	for (auto it = lines.begin(); it != lines.end(); ++it)
	{
		decltype(lines.begin()) nextIt;
		if (it == std::prev(lines.end()))
		{
			nextIt = lines.begin();
		}
		else
		{
			nextIt = std::next(it);
		}

		auto triangle = Triangle(*it, *nextIt);
		unsigned int color;
		if (opacity != 1.0)
		{
			color = 0xffffffff;
		}
		else
		{
			color = 0xff000000u + static_cast<unsigned int>(colorDis(gen));
		}
		renderTriangle(triangle, drawable, color, opacity);
	}
}

void drawRandomTriangles(Rect& panel, Drawable* drawable, double opacity = 1.0)
{
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<> dist(0, 299);
	std::uniform_int_distribution<> colorDis(0x00000000, 0x00ffffff);

	std::vector<Triangle> triangles;
	triangles.reserve(20);

	std::generate_n(std::back_inserter(triangles), 20, [&dist, &gen, &panel]
	{
		auto p1 = Point{ dist(gen), dist(gen) };
		auto p2 = Point{ dist(gen), dist(gen) };
		auto p3 = Point{ dist(gen), dist(gen) };
		return Triangle(std::array<Point, 3>{p1, p2, p3}, &panel);
	});

	std::for_each(triangles.begin(), triangles.end(), [&colorDis, &gen, drawable, opacity](auto& triangle)
	{			
		unsigned int color;
		if (opacity != 1.0)
		{
			color = 0xffffffff;
		}
		else
		{
			color = 0xff000000u + static_cast<unsigned int>(colorDis(gen));
		}
		renderTriangle(triangle, drawable, color, opacity);
	});
}

auto generatePointGrid(int margin, int triangleLength)
{
	auto currentY = margin;
	std::vector<std::vector<Point>> pointGrid;
	pointGrid.reserve(10);
	std::generate_n(std::back_inserter(pointGrid), 10, [&currentY, margin, triangleLength]
	{
		std::vector<Point> pointRow;
		pointRow.reserve(10);
		auto currentX = margin;
		std::generate_n(std::back_inserter(pointRow), 10, [&currentX, currentY, margin, triangleLength]
		{
			auto point = Point{ currentX, currentY };
			currentX += triangleLength;
			return point;
		});
		currentY += triangleLength;
		return pointRow;
	});
	return pointGrid;
}

void drawPointGrid(Rect& panel, const std::vector<std::vector<Point>>& pointGrid, Drawable* drawable, double opacity = 1.0)
{
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<> colorDis(0x00000000, 0x00ffffff);
	auto count = 1;
	std::ofstream outFile("log.txt", std::ios_base::out);

	for (auto row = 0; row < 9; ++row)
	{
		for (auto col = 0; col < 9; ++col)
		{
			unsigned int color;
			if (opacity != 1.0)
			{
				color = 0xffffffff;
			}
			else
			{
				color = 0xff000000u + static_cast<unsigned int>(colorDis(gen));
			}
			auto triangle = Triangle(std::array<Point, 3>{pointGrid[row][col], pointGrid[row][col + 1], pointGrid[row + 1][col + 1]}, &panel);
			renderTriangle(triangle, drawable, color, opacity);
			drawable->updateScreen();
			outFile << "Drawing triangle " << count++ << "- ";
			for (auto point : triangle.vertices())
			{
				outFile << point.x << ":" << point.y << "   ";
			}
			outFile << color << "   ";
			auto totalX = 0;
			auto totalY = 0;
			for (auto point : triangle.vertices())
			{
				auto globalPoint = point.toGlobalCoordinate();
				totalX += globalPoint.x;
				totalY += globalPoint.y;
			}
			auto midX = static_cast<int>(totalX / 3.0);
			auto midY = static_cast<int>(totalY / 3.0);
			auto pixel = drawable->getPixel(midX, midY);
			outFile << "Center color " << pixel;
			if (color == pixel)
			{
				outFile << " MATCH";
			}
			else
			{
				outFile << " NOT MATCH";
			}
			outFile << "\n";
			

			if (opacity != 1.0)
			{
				color = 0xffffffff;
			}
			else
			{
				color = 0xff000000u + static_cast<unsigned int>(colorDis(gen));
			}
			triangle = Triangle(std::array<Point, 3>{pointGrid[row][col], pointGrid[row + 1][col + 1], pointGrid[row + 1][col]}, &panel);
			renderTriangle(triangle, drawable, color, opacity);
			drawable->updateScreen();

			outFile << "Drawing triangle " << count++ << "- ";
			for (auto point : triangle.vertices())
			{
				outFile << point.x << ":" << point.y << "   ";
			}
			outFile << color << "   ";
			totalX = 0;
			totalY = 0;
			for (auto point : triangle.vertices())
			{
				auto globalPoint = point.toGlobalCoordinate();
				totalX += globalPoint.x;
				totalY += globalPoint.y;
			}
			midX = static_cast<int>(totalX / 3.0);
			midY = static_cast<int>(totalY / 3.0);
			pixel = drawable->getPixel(midX, midY);
			outFile << "Center color " << pixel;
			if (color == pixel)
			{
				outFile << " MATCH";
			}
			else
			{
				outFile << " NOT MATCH";
			}
			outFile << "\n";
		}
	}
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
	draw_rect(panel3.x, panel3.y, panel3.right(), panel3.bottom(), 0xffffffff);

	Rect panel4{ 400, 400, 300, 300 };
	draw_rect(panel4.x, panel4.y, panel4.right(), panel4.bottom(), 0xff000000);


    switch(pageNumber % 5) 
	{
		case 1:
		{
			drawStarburst(panel1, drawable, 90, 125, DDALineRenderer);
			drawStarburst(panel2, drawable, 90, 125, BresenhamLineRenderer);
			drawStarburst(panel3, drawable, 90, 125, DDALineRenderer, BresenhamLineRenderer);
			renderLine(Line{ Point{ 99, 14, &panel4 }, Point{ 129, 16, &panel4 } }, drawable, DDALineRenderer);
			renderLine(Line{ Point{ 99, 114, &panel4 }, Point{ 129, 42, &panel4 } }, drawable, DDALineRenderer);

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
			// Panel 1
			drawCircularTriangles(panel1, drawable);

			// Panel 2
			// Generate points
			auto margin = 13;
			auto triangleLength = 30;
			auto pointGrid = generatePointGrid(margin, triangleLength);
			drawPointGrid(panel2, pointGrid, drawable);

			// Panel 3
			// Shift points
			std::random_device device;
			std::mt19937 gen(device());
			std::uniform_int_distribution<> shiftDis(0, 12);

			std::for_each(pointGrid.begin(), pointGrid.end(), [&shiftDis, &gen] (auto& currentRow)
			{
				std::for_each(currentRow.begin(), currentRow.end(), [&shiftDis, &gen](auto& point)
				{
					point.x += (shiftDis(gen) - 6);
					point.y += (shiftDis(gen) - 6);
				});
			});
			drawPointGrid(panel3, pointGrid, drawable);

			// Panel 4
			//drawRandomTriangles(panel4, drawable);
			auto triangle = Triangle(std::array<Point, 3>{Point{ 79, 10, nullptr }, Point{ 101, 24, nullptr }, Point{ 109, 31, nullptr }}, &panel4);
			renderTriangle(triangle, drawable, 0xffffffff);

		} break;
		// fall through...
		default:
		{
			// Panel 1
			drawCircularTriangles(panel1, drawable, 0.14);

			// Panel 2
			// Generate points
			auto margin = 13;
			auto triangleLength = 30;
			auto pointGrid = generatePointGrid(margin, triangleLength);
			drawPointGrid(panel2, pointGrid, drawable, 0.14);

			// Panel 3
			// Shift points
			std::random_device device;
			std::mt19937 gen(device());
			std::uniform_int_distribution<> shiftDis(0, 24);

			std::for_each(pointGrid.begin(), pointGrid.end(), [&shiftDis, &gen](auto& currentRow)
			{
				std::for_each(currentRow.begin(), currentRow.end(), [&shiftDis, &gen](auto& point)
				{
					point.x += (shiftDis(gen) - 12);
					point.y += (shiftDis(gen) - 12);
				});
			});
			drawPointGrid(panel3, pointGrid, drawable, 0.14);

			// Panel 4
			drawRandomTriangles(panel4, drawable, 0.14);
		} break;
	}

	drawable->updateScreen();   // you must call this to make the display change.

}

void Client::draw_rect(int x1, int y1, int x2, int y2, unsigned int color) 
{
    for(int x = x1; x<x2; x++) 
	{
        for(int y=y1; y<y2; y++) 
		{
            drawable->setPixel(x, y, color);
        }
    }
}
