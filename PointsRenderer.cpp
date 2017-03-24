#include "PointsRenderer.hpp"

namespace PointsRenderer
{
	Color getCurrentColor(int x, int y, Drawable* surface)
	{
		return Color{ surface->getPixel(x, y) };
	}

	bool pointInRect(const Point& point, const Rect& rect)
	{
		return point.x >= rect.x && point.x < rect.right() && point.y >= rect.y && point.y < rect.bottom();
	}

	// Draw to surface
	void drawToSurface(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint)
	{
		drawSurface->setPixel(screenPoint.x, screenPoint.y, colorToPaint.asUnsigned());
	}

	void drawPixel(const Point& screenPoint, Drawable* drawSurface, const Color& colorToPaint, Matrix2D<int>& zBuffer, const Rect& viewPort, const Camera& camera)
	{
		if (screenPoint.z < zBuffer[screenPoint.x - viewPort.x][screenPoint.y - viewPort.y] && screenPoint.z >= camera.near)
		{
			zBuffer[screenPoint.x - viewPort.x][screenPoint.y - viewPort.y] = screenPoint.z;
			drawToSurface(screenPoint, drawSurface, colorToPaint);
		}
	}

	void PointsRenderer::renderPoints(const std::vector<Point>& points, Drawable * drawSurface, Matrix2D<int>& zBuffer, const Rect & viewPort, const Camera& camera)
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

