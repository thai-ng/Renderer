#include "PointsRenderer.hpp"

namespace PointsRenderer
{
	Color getCurrentColor(int x, int y, Drawable* surface)
	{
		return Color{ surface->getPixel(x, y) };
	}

	bool pointInRect(const Point& point, const Rect& rect)
	{
		auto x = std::round(point.x);
		auto y = std::round(point.y);
		return x >= rect.x && x < rect.right() && y >= rect.y && y < rect.bottom();
	}

	// Draw to surface
	void drawToSurface(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint)
	{
		drawSurface->setPixel(static_cast<int>(std::round(screenPoint.x)), static_cast<int>(std::round(screenPoint.y)), colorToPaint.asUnsigned());
	}

	void drawPixel(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint, Matrix2D<double>& zBuffer, const Rect& viewPort, const Camera& camera)
	{
		auto x = static_cast<int>(std::round(screenPoint.x - viewPort.x));
		auto y = static_cast<int>(std::round(screenPoint.y - viewPort.y));
		if (screenPoint.z <= zBuffer[x][y] && screenPoint.z >= camera.near)
		{
			zBuffer[x][y] = screenPoint.z;
			drawToSurface(screenPoint, drawSurface, colorToPaint);
		}
	}

	void PointsRenderer::renderPoints(const std::vector<Point>& points, Drawable * drawSurface, Matrix2D<double>& zBuffer, const Rect & viewPort, const Camera& camera)
	{
		for (const auto& point : points)
		{
			if (pointInRect(point, viewPort))
			{
				drawPixel(point, drawSurface, point.color, zBuffer, viewPort, camera);
			}
		}
	}
}

