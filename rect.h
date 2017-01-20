#pragma once
struct Rect
{
	int x;
	int y;
	int width;
	int height;
	int bottom() { return y + height; }
	int right() { return x + width; }
};
