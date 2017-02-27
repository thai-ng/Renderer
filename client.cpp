#include "client.h"

#include <iostream>

#include "assignment2.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}

void Client::nextPage() 
{
	assignment2::doNextPage(this);
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
