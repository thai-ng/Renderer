#include "client.h"

#include <iostream>
#include "assignment1.h"
#include "assignment2.h"
#include "SimpFile.h"
#include "Matrix.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}

void Client::nextPage() 
{
	Matrix<3, 3, int> matrix{ 1, 0, 0, 
							  0, 2, 0,
							  0, 0, 3 };
	Matrix<3, 3, int> permutation{ 0, 1, 0,
								   0, 0, 1,
								   1, 0, 0 };
	
	auto mult = permutation * matrix;
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
