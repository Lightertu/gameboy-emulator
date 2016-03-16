#ifndef GPU_GAMEBOY_H_
#define GPU_GAMEBOY_H_
#include <iostream>
#include "cpu.hpp"
#include "display.hpp"
#include <SDL2/SDL.h>

class GPU {
public:
  GPU();
  virtual~GPU();

  static void gpu_step(SDL_Renderer *gRenderer);
  static int mode;
  static int g_clock;
  static int render_line;
  static uint8_t isBackGroundOn;
  static uint8_t isSpriteOn;
  static uint8_t isLargeSprit;
  static void renderScanLine(SDL_Renderer *gRenderer,int line);
  static void updateLCDcontrol(uint8_t lcdByte);
/*
  const static int SCREEN_WIDTH = 160;
  const static int SCREEN_HEIGHT = 144;
  static SDL_Window *gWindow;
  static SDL_Renderer *gRenderer;
*/
  static bool initializeWindow();

};

#endif
