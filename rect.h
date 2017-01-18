struct Rect
{
	int x;
	int y;
	int width;
	int height;
	int bottom() { return y + (height - y); }
	int right() { return x + (width - x); }
};