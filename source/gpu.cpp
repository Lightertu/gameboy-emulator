#include <iostream>
#include "../include/gpu.hpp"
#include "../include/memory.hpp"
#include "../include/display.hpp"
#include <SDL2/SDL.h>
#include <vector>

#define OAM 2
#define VRAM_i 3
#define HBLANK 0
#define VBLANK 1

/*
CITATIONS:
SDL framework draws heavily from Imrannazar's javascript gameboy emulator blog found here:
http://imrannazar.com/GameBoy-Emulation-in-JavaScript
*/


uint8_t GPU::isBackGroundOn;
uint8_t GPU::isSpriteOn;
uint8_t GPU::isLargeSprit;

GPU::GPU(){};
GPU::~GPU(){};

int GPU::mode = 0;
int GPU::g_clock = 0;
int GPU::render_line = 0;
//uint32_t_t pixel_lines[160 * 144];

extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;
extern SDL_Surface *gScreenSurface;
extern SDL_Surface *gTempScreen;
extern SDL_Texture* newTexture;
extern uint32_t pallatte[4];

int width_screen = 160;
int pitch_screen = 144;
uint32_t color_screen [160 * 144];

int scan_line[160];

int pitch, w, h;
void *pixels;
uint32_t *upixels;


void GPU::updateLCDcontrol(uint8_t lcdByte) {
    GPU::isBackGroundOn      = (lcdByte & 0x01) ? 1 : 0;
    GPU::isSpriteOn          = (lcdByte & 0x02) ? 1 : 0;
    GPU::isLargeSprit        = (lcdByte & 0x04) ? 1 : 0;

    /* if necessary more can be updated */
    // isUpperBGtileMap    = (lcdByte & 0x01) ? 1 : 0;
    // isBackGroundOn      = (lcdByte & 0x01) ? 1 : 0;
    // isBackGroundOn      = (lcdByte & 0x01) ? 1 : 0;
}

void GPU::gpu_step(SDL_Renderer *gRenderer){
  render_line = Memory::read(0xFF44);
  GPU::g_clock += CPU::getLastTick();


  if(GPU::mode == OAM){
	if(GPU::g_clock >= 80){
	  GPU::g_clock = 0;
	  GPU::mode = VRAM_i;
	}
  } else if(GPU::mode == VRAM_i){
	if(GPU::g_clock >= 172){
	  GPU::g_clock = 0;
	  GPU::mode = HBLANK;

	  GPU::renderScanLine(gRenderer,Memory::read(0xFF44));
	}
  } else if(GPU::mode == HBLANK){
	if(GPU::g_clock >= 204){
	  GPU::g_clock = 0;
	  Memory::write(0xFF44, Memory::read(0xFF44) + 1);

	  if(Memory::read(0xFF44)== 143){
		GPU::mode = VBLANK;
		u_int8_t i_enable = Memory::read(0xFFFF);
		u_int8_t i_flags = Memory::read(0xFF0F);
		if(i_enable & 0x01){
		  i_flags |= 0x01;
		  Memory::write(0xFF0F, i_flags);
		}

		SDL_QueryTexture(newTexture, NULL, NULL, &w, &h);
		SDL_LockTexture(newTexture, NULL, &pixels, &pitch);
		uint32_t *upixels = (uint32_t*) pixels;
		memcpy(upixels, &color_screen, (sizeof(uint32_t) * 160 * 144));
		SDL_UnlockTexture(newTexture);
		SDL_RenderCopy( gRenderer, newTexture, NULL, NULL );
		SDL_RenderPresent( gRenderer );
	  } else {
		GPU::mode = OAM;
	  }
	}
  } else if(GPU::mode == VBLANK){
	if(GPU::g_clock >= 456){
	  Memory::write(0xFF44, Memory::read(0xFF44) + 1);
	  if(Memory::read(0xFF44)> 153){
		GPU::mode = OAM;
		Memory::write(0xFF44, 0);
	  }
	  GPU::g_clock = 0;
	}
  }
}

void GPU::renderScanLine(SDL_Renderer *gRenderer, int line){
  uint8_t LCDF		   = Memory::IO[0x40];
  uint8_t scrollY		= Memory::IO[0x42];
  uint8_t scrollX		= Memory::IO[0x43];
  uint8_t scanLine	   = Memory::IO[0x44];

  int backgroundTileMap	 = (LCDF & 0x10) ? 1 : 0;
  int backgroundMap		 = (LCDF & 0x08) ? 1 : 0;
  int switchLCD			 = (LCDF & 0x80) ? 1 : 0;
  int switchBackground	  = (LCDF & 0x01) ? 1 : 0;

  /* if the background is on */
  if ( GPU::isBackGroundOn ) {
	std::vector<uint8_t> scanRowPixels;

	int mapOffset = backgroundMap ? 0x1C00 : 0x1800;
	mapOffset += (((scanLine + scrollY) & 255) >> 3) << 5;

	int lineOffset = (scrollX >> 3);
	int xCoord = scrollX & 7;
	int yCoord = (scanLine + scrollY) & 7;

	int SDLoffset = scanLine * 160;

	uint32_t color;
	int color_temp;
	uint16_t tile = Memory::VRAM[mapOffset+lineOffset];
	for(int i = 0; i < 160; i++) {
          int temp_color = Memory::tileCollection[tile][yCoord][xCoord];
          scan_line[i] = temp_color;
 		color_screen[SDLoffset++] = pallatte[temp_color];

		xCoord += 1;
		if(xCoord == 8){
		  xCoord = 0;
		  lineOffset = (lineOffset + 1) & 31;
		  tile = Memory::VRAM[mapOffset + lineOffset];
		}
	}
        for(int i = 0; i < 40; i++){
          Sprite* cur_sprite = &Memory::spriteCollection[i];
          int adj_x = cur_sprite->xpos;
          int adj_y = cur_sprite->ypos;

          if((adj_y <= scanLine) && ((adj_y + 8) > scanLine)){
            SDLoffset = scanLine * 160 + adj_x;
            uint8_t tileRow;
            if(cur_sprite->yflip){
              tileRow = 7 - (scanLine - adj_y);
            }else{
              tileRow = scanLine - adj_y;
            }
            for(int x = 0; x < 8; x++){
              if((adj_x + x >= 0) && (adj_x + x < 160) && (~cur_sprite->priority || !scan_line[adj_x + x])){
                uint8_t color;
                if(cur_sprite->xflip){
                  color = Memory::tileCollection[cur_sprite->tile][tileRow][7 - x];
                }else{
                  color = Memory::tileCollection[cur_sprite->tile][tileRow][x];
                }
                if(color){ //if there is a color to plot
                  color_screen[SDLoffset] = pallatte[color];
                }
                SDLoffset++;
              }
              //SDLoffset++;
            }
        }
     }
  }
}
