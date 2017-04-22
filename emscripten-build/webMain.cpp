// #include <fstream>
// #include <emscripten.h>
// #include <SDL/SDL.h>

// int main()
// {
//     SDL_Init(SDL_INIT_VIDEO);
//     SDL_Surface* screen = SDL_SetVideoMode(650, 650, 32, SDL_SWSURFACE);

//     return 0;
// }

#include <SDL/SDL.h>

#include <emscripten.h>
#include "SDLDrawable.hpp"
#include "client.h"

int main() {

  SDLDrawable drawable(750, 750);
  Client client(&drawable, "test.simp");
  client.nextPage();

  return 0;
}