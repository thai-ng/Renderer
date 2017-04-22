#include "SDLDrawable.hpp"

SDLDrawable::SDLDrawable(int w, int h) : width(w), height(h)
{
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);    
}


SDLDrawable::~SDLDrawable()
{
      SDL_Quit();
}

struct ColorTriplet
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

void SDLDrawable::updateScreen()
{
    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    SDL_Flip(screen); 
    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
}

unsigned int SDLDrawable::getPixel(int, int) 
{
    return 0u;
};

ColorTriplet getColor(unsigned int color)
{
    return ColorTriplet {
        static_cast<unsigned char>(color << 8 >> 24),
        static_cast<unsigned char>(color << 16 >> 24),
        static_cast<unsigned char>(color << 24 >> 24)
    };

}

void SDLDrawable::setPixel(int x, int y, unsigned int color)
{
    auto colorTriplet = getColor(color);
     *((Uint32*)screen->pixels + y * width + x) = SDL_MapRGBA(screen->format, colorTriplet.r, colorTriplet.g, colorTriplet.b, 255);
}
