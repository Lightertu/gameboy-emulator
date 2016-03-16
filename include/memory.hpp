#ifndef MEMORY_H_
#define MEMORY_H_
#include <string>
#include <cstdint>
#include <iostream>
#include "cpu.hpp"
#include "key_presses.hpp"
//MEMORY BLOCKS
typedef struct _Sprite {
	// 3 bytes
	uint8_t ypos; // X-coordinate of top-left corner (Value stored is X-coordinate minus 8)
	uint8_t xpos; // Y-coordinate of top-left corner (Value stored is Y-coordinate minus 16)
	uint8_t tile; // tile number
	// last byte splited into  bits
	uint8_t priority : 1;     // 0: above   1: under
	uint8_t yflip    : 1;	  // 0: normal  1: vertically flipped
	uint8_t xflip    : 1;     // 0: normal  1: horizontally  flipped
	uint8_t palette  : 1;     // 0: OBJ palette #0 1: 0: OBJ palette #1
} Sprite;

class Memory{
public:
	Memory();
	virtual ~Memory();

	static std::uint8_t bootMem[0x100]; //256kb bootstrap file
	static std::uint8_t cartROM[0x7FFF+1]; //0000 to 7FFF all game files
	static std::uint8_t VRAM[0x1FFF+1]; //8000-9FFF Video RAM
	static std::uint8_t SRAM[0x1FFF+1]; // save states
	static std::uint8_t WRAM[0x1FFF+1]; //C000-DFFF this is similar to VRAM, "faster"
	static std::uint8_t ECHO[0x1DFF+1]; //E000-FDFF Echo RAM
	static std::uint8_t OAM[0x009F+1]; //FE00-FE9F Object Attribute Memory, meant for sprites. Non in tetris.
	static std::uint8_t IO[0x007F+1]; //FF00-FF7F Hardware I/O Registers
	static std::uint8_t HRAM[0x007F+1]; //FF80-FFFF High RAM
	static std::uint8_t cartRAM[0x1FFF+1]; // A000-BFFF if available.
	static std::uint8_t unusedMemory[0x005F+1];
	static std::uint8_t tileCollection[0x0180][0x08][0x08];
	static Sprite spriteCollection[0x0028];
	static Sprite* getSprite(uint8_t index);
	static std::uint8_t getTilePixel(int tileIndex, int x, int y);
	static std::uint8_t read(uint16_t index);
	static void write(uint16_t index, std::uint8_t hexValue);
	static void cleanMemory();
	static void initializeMemory();
	static void printMemory(uint16_t index1, uint16_t index2);
	static void updateTileCollection(uint16_t address);
	static void updateSpriteCollection(uint16_t address, uint8_t value);
	static void printTiles();
	static uint8_t** getSpriteBlock(uint16_t address);
	static void printSingleTile(uint16_t tile);
private:

};





#endif
