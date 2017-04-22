#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "drawable.h"
#include "pageturner.h"

class Client : public PageTurner
{
public:
	Client(Drawable *drawable);
	Client(Drawable *drawable, const std::string& fileName);
    void nextPage();
	Drawable* getDrawable() const;
private:
    Drawable *drawable;
    void draw_rect(int x1, int y1, int x2, int y2, unsigned int color);

	std::string _fileName;
};

#endif // CLIENT_H
