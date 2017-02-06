#include "client.h"
#include "assignment1.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}

void Client::nextPage() 
{
	static int pageNumber = 0;
	pageNumber++;
	std::cout << "PageNumber " << pageNumber << std::endl;

	Rect bg{ 0, 0, 750, 750 };
	draw_rect(bg.x, bg.y, bg.right(), bg.bottom(), 0xffffffff);

	Rect viewPort{ 50, 50, 650, 650 };
	draw_rect(viewPort.x, viewPort.y, viewPort.right(), viewPort.bottom(), 0x00000000);
}

Drawable* Client::getDrawable() const
{
	return drawable;
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
