#ifndef DISPLAY_SDL_WINDOW_H_
#define DISPLAY_SDL_WINDOW_H_

#include <SDL2/SDL.h>



//SDL_Window *gWindow;
//SDL_Renderer *gRenderer;

int initializeWindow();


// Lazy Foo code
Uint32 get_pixel( SDL_Surface *surface, int x, int y);
void put_pixel( SDL_Surface *surface, int x, int y, Uint32 pixel);
#endif
