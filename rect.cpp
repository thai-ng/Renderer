#include "primitives.h"

int Rect::bottom() const { return y + height; }
int Rect::right() const { return x + width; }

Point Rect::center() const
{
	return Point((width / 2), (height / 2), 0, this, Color{ 0, 0, 0 });
}
