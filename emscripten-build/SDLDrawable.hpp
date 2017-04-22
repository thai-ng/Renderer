#include "drawable.h"
#include <SDL/SDL.h>

class SDLDrawable : public Drawable
{
public:
    SDLDrawable(int w, int h);
    ~SDLDrawable();
    void setPixel(int x, int y, unsigned int color);
    unsigned int getPixel(int x, int y);
    void updateScreen();

private:
    SDLDrawable(const SDLDrawable&) = delete;
    SDLDrawable(SDLDrawable&&) = delete;
    SDLDrawable& operator=(const SDLDrawable&) = delete;
    SDLDrawable& operator=(SDLDrawable&&) = delete;
    SDL_Surface* screen;
    int width;
    int height;
};