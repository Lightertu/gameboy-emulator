#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 160; //160
const int SCREEN_HEIGHT = 144; //144

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gTempScreen = NULL;
SDL_Texture* newTexture = NULL;

SDL_Texture* mTexture;
void* mPixels;

Uint32 pallatte[4];

int initializeWindow()
{


  //Initialization flag
  bool success = true;

  //Create window
  gWindow = SDL_CreateWindow( "QUACKPACK", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE );
  if( gWindow == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
  else
    {
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
      gScreenSurface = SDL_GetWindowSurface(gWindow);
      newTexture = SDL_CreateTexture( gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, gScreenSurface->w, gScreenSurface->h );
      int zoom = 7;
      SDL_SetWindowSize(gWindow, 160 * zoom, 144 * zoom);

      //gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL )
        {
          printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
          success = false;
        }
      else
        {
          //Initialize renderer color
          SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }

    }


  // pallatte[0] = SDL_MapRGB(gScreenSurface->format, 255,255,255);
  // pallatte[1] = SDL_MapRGB(gScreenSurface->format, 192,192,192);
  // pallatte[2] = SDL_MapRGB(gScreenSurface->format, 96,96,96);
  // pallatte[3] = SDL_MapRGB(gScreenSurface->format, 0,0,0);
  pallatte[0] = SDL_MapRGB(gScreenSurface->format, 255 ,255,255);
  pallatte[1] = SDL_MapRGB(gScreenSurface->format, 192,192,192);
  pallatte[2] = SDL_MapRGB(gScreenSurface->format, 96,96,96);
  pallatte[3] = SDL_MapRGB(gScreenSurface->format, 0,0,0);

  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear( gRenderer );
  return success;
}


Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    return *p;
    break;

  case 2:
    return *(Uint16 *)p;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
    break;

  case 4:
    return *(Uint32 *)p;
    break;

  default:
    return 0;       /* shouldn't happen, but avoids warnings */
  }
}

void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    *p = pixel;
    break;

  case 2:
    *(Uint16 *)p = pixel;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;

  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}
