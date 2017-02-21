#pragma once

#include <random>
#include <iostream>
#include <vector>

#include "client.h"
#include "polygonRenderer.h"

namespace assignment2
{
	auto generatePointGrid(int margin, int triangleLength)
	{
		auto currentY = margin;
		std::vector<std::vector<Point>> pointGrid;
		pointGrid.reserve(10);
		std::random_device device;
		std::mt19937 gen(device());
		std::uniform_int_distribution<unsigned int> colorDis(0x00000000u, 0x00ffffffu);

		std::generate_n(std::back_inserter(pointGrid), 10, [&currentY, &colorDis, &gen, margin, triangleLength]
		{
			std::vector<Point> pointRow;
			pointRow.reserve(10);
			auto currentX = margin;
			std::generate_n(std::back_inserter(pointRow), 10, [&currentX, &colorDis, &gen, currentY, margin, triangleLength]
			{
				auto color = colorDis(gen);
				auto point = Point{ currentX, currentY, nullptr, Color(color) };
				currentX += triangleLength;
				return point;
			});
			currentY += triangleLength;
			return pointRow;
		});
		return pointGrid;
	}

	void drawPointGridWireFrame(Rect& panel, const std::vector<std::vector<Point>>& pointGrid, Drawable* drawable)
	{
		for (auto row = 0; row < 9; ++row)
		{
			for (auto col = 0; col < 9; ++col)
			{

				auto triangle = Polygon(std::vector<Point>{pointGrid[row][col], pointGrid[row][col + 1], pointGrid[row + 1][col + 1]}, &panel);
				renderPolygonWireframe(triangle, drawable);

				triangle = Polygon(std::vector<Point>{pointGrid[row][col], pointGrid[row + 1][col + 1], pointGrid[row + 1][col]}, &panel);
				renderPolygonWireframe(triangle, drawable);
			}
		}
	}

	void drawPointGrid(Rect& panel, const std::vector<std::vector<Point>>& pointGrid, Drawable* drawable)
	{
		std::random_device device;
		std::mt19937 gen(device());
		std::uniform_int_distribution<> colorDis(0x00000000, 0x00ffffff);
		for (auto row = 0; row < 9; ++row)
		{
			for (auto col = 0; col < 9; ++col)
			{
				auto triangle = Triangle(std::array<Point, 3>{pointGrid[row][col], pointGrid[row][col + 1], pointGrid[row + 1][col + 1]}, &panel);
				renderTriangle(triangle, drawable);

				triangle = Triangle(std::array<Point, 3>{pointGrid[row][col], pointGrid[row + 1][col + 1], pointGrid[row + 1][col]}, &panel);
				renderTriangle(triangle, drawable);
			}
		}
	}

	void draw_rect(Client* client, int x1, int y1, int x2, int y2, unsigned int color)
	{
		for (int x = x1; x<x2; x++)
		{
			for (int y = y1; y<y2; y++)
			{
				client->getDrawable()->setPixel(x, y, color);
			}
		}
	}

	void doNextPage(Client* client)
	{
		static int pageNumber = 0;
		pageNumber++;
		std::cout << "PageNumber " << pageNumber << std::endl;

		Rect bg{ 0, 0, 750, 750 };
		draw_rect(client, bg.x, bg.y, bg.right(), bg.bottom(), 0xffffffff);

		Rect viewPort{ 50, 50, 650, 650 };
		draw_rect(client, viewPort.x, viewPort.y, viewPort.right(), viewPort.bottom(), 0x00000000);

		switch (pageNumber % 7)
		{
			// Page 1
			case 1:
			{
				auto margin = 13;

				auto triangleLength = (viewPort.width - (margin * 2)) / 9;;
				auto pointGrid = generatePointGrid(margin, triangleLength);

				// Panel 3
				// Shift points
				std::random_device device;
				std::mt19937 gen(device());
				std::uniform_int_distribution<> shiftDis(0, 12);

				std::for_each(pointGrid.begin(), pointGrid.end(), [&shiftDis, &gen](auto& currentRow)
				{
					std::for_each(currentRow.begin(), currentRow.end(), [&shiftDis, &gen](auto& point)
					{
						point.x += (shiftDis(gen) - 6);
						point.y += (shiftDis(gen) - 6);
					});
				});
				drawPointGridWireFrame(viewPort, pointGrid, client->getDrawable());
			} break;

			// Page 2
			case 2:
			{
				auto margin = 13;

				auto triangleLength = (viewPort.width - (margin * 2)) / 9;;
				auto pointGrid = generatePointGrid(margin, triangleLength);

				// Panel 3
				// Shift points
				std::random_device device;
				std::mt19937 gen(device());
				std::uniform_int_distribution<> shiftDis(0, 12);

				std::for_each(pointGrid.begin(), pointGrid.end(), [&shiftDis, &gen](auto& currentRow)
				{
					std::for_each(currentRow.begin(), currentRow.end(), [&shiftDis, &gen](auto& point)
					{
						point.x += (shiftDis(gen) - 6);
						point.y += (shiftDis(gen) - 6);
					});
				});
				drawPointGrid(viewPort, pointGrid, client->getDrawable());
			} break;

			case 3:
			{
				Point p1{ 50, 50, nullptr, Color(255, 0, 0) };
				Point p2{ 700, 50, nullptr, Color(0, 255, 0) };
				Point p3{ 50, 700, nullptr, Color(0, 0, 255) };
				renderPolygon(std::vector<Point>{p1, p2, p3}, client->getDrawable());
			} break;

			default:
			{

			}break;
		}
		client->getDrawable()->updateScreen();   // you must call this to make the display change.

	}
}
