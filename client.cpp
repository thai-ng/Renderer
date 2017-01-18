#include <iostream>
#include "client.h"
#include "lineRenderer.h"
#include "rect.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}

void Client::nextPage() {
    static int pageNumber = 0;
    pageNumber++;
    std::cout << "PageNumber " << pageNumber << std::endl;

    switch(pageNumber % 4) {
    case 1:
	{
		Rect bg{ 0, 0, 750, 750 };
        draw_rect(bg.x, bg.y, bg.right(), bg.bottom(), 0xffffffff);

		Rect panel1{ 50, 50, 300, 300 };
        draw_rect( panel1.x, panel1.y, panel1.right(), panel1.bottom(), 0xff000000);

		Rect panel2{ 400, 50, 300, 300 };
		draw_rect(panel2.x, panel2.y, panel2.right(), panel2.bottom(), 0xff000000);

		Rect panel3{ 50, 400, 300, 300 };
		draw_rect(panel3.x, panel3.y, panel3.right(), panel3.bottom(), 0xff000000);

		Rect panel4{ 400, 400, 300, 300 };
		draw_rect(panel4.x, panel4.y, panel4.right(), panel4.bottom(), 0xff000000);

        renderLine(Point{50, 60}, Point{60, 200}, drawable, 0xffffffff, DDALineRenderer);
        renderLine(Point{60, 60}, Point{70, 200}, drawable, 0xffffff00, BresenhamLineRenderer);
        drawable->updateScreen();   // you must call this to make the display change.
	} break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        // fall through...
    default:
        draw_rect(0, 0, 750, 750, 0xffffffff);
        draw_rect(400, 400, 700, 700, 0xff00ff40);
        drawable->updateScreen();
    }
}

void Client::draw_rect(int x1, int y1, int x2, int y2, unsigned int color) {
    for(int x = x1; x<x2; x++) {
        for(int y=y1; y<y2; y++) {
            drawable->setPixel(x, y, color);
        }
    }
}
