#include <iostream>
#include "../include/memory.hpp"

Memory::Memory(){};
Memory::~Memory(){};

#define $A 1
#define $F 0
#define $B 3
#define $C 2
#define $D 5
#define $E 4
#define $H 7
#define $L 6

#define $AF 0
#define $BC 1
#define $DE 2
#define $HL 3
#define $SP 4
#define $PC 5
 

/* initializing all memory */
std::uint8_t Memory::bootMem[0x100];
std::uint8_t Memory::cartROM[0x7FFF+1]; //0000 to 7FFF all game files
std::uint8_t Memory::VRAM[0x1FFF+1]; //8000-9FFF Video RAM
std::uint8_t Memory::SRAM[0x1FFF+1]; // save states
std::uint8_t Memory::WRAM[0x1FFF+1]; //C000-DFFF this is similar to VRAM, "faster"
std::uint8_t Memory::ECHO[0x1DFF+1]; //E000-FDFF Echo RAM
std::uint8_t Memory::OAM[0x009F+1]; //FE00-FE9F Object Attribute Memory, meant for sprites. Non in tetris.
std::uint8_t Memory::IO[0x007F+1]; //FF00-FF7F Hardware I/O Registers
std::uint8_t Memory::HRAM[0x007F+1]; //FF80-FFFE High RAM
std::uint8_t Memory::cartRAM[0x1FFF+1];
std::uint8_t Memory::unusedMemory[0x005F+1];
std::uint8_t Memory::tileCollection[0x0180][0x08][0x08];
Sprite Memory::spriteCollection[0x0028];



/* begin memory functions */
void Memory::initializeMemory(){
	std::copy(std::begin(DataBank::bootstrapFile), std::end(DataBank::bootstrapFile), std::begin(Memory::bootMem));
	for (uint16_t i = 0; i < 0x0028; i++) {
		Memory::spriteCollection[i] = {
			0, // xpos
			0, // ypos
			0, // tile
			0, // priority
			0, // yflip
			0, // xflip
			0  // palette
		};
	}
}


// TODO figure out how to crash code for bad values.
void Memory::cleanMemory(){
	for(int i = 0; i < 0xFFFF; i++){
		if (i < 0xFEA0 && i > 0xFEFF)
			Memory::write(0x00, i);
	}
}

void Memory::printMemory(uint16_t index1, uint16_t index2){
	std::cout << "PRINTING MEMORY FROM "<< std::hex << (int)index1 << " to " << (int)index2 << std::endl;
	int lineCount = 0;
	for(int i = index1; i <= index2; i++){
		if(lineCount == 15){ std::cout << "" << std::endl; lineCount = 0;}
		if(read(index1) != 0){
			std::cout << std::hex << (int)index1 << ": " << (int)read(index1) << std::endl;
			lineCount++;
		}
		index1++;
	}
}

void Memory::write(uint16_t index, std::uint8_t hexValue){
  //std::cout << "writing: " << (std::hex) << (int) index << std::endl;
	if(index >= 0x0000 && index <= 0x7FFF){
		if(index < 0x100){ bootMem[index] = hexValue; }
		cartROM[index] = hexValue;
	} else if(index >= 0x8000 && index <= 0x9FFF){
		VRAM[index - 0x8000] = hexValue;
		if (index < 0x9800) updateTileCollection(index); // added update tile
	} else if(index >= 0xA000 && index <=0xBFFF){
		index = index - 0xA000;
		cartRAM[index] = hexValue;
	} else if(index >= 0xC000 && index <= 0xDFFF){
		index = index - 0xC000;
		WRAM[index] = hexValue;
	} else if(index >= 0xE000 && index <= 0xFDFF){
	        Memory::write(index-0x2000, hexValue); //copy to other memory from echo
		ECHO[index - 0xE000] = hexValue;
	} else if(index >= 0xFE00 && index <= 0xFE9F){
		updateSpriteCollection(index - 0xFE00, hexValue);
		OAM[index - 0xFE00] = hexValue;
	} else if (index >= 0xFEA0 && index <= 0xFEFF){
		unusedMemory[index - 0xFEA0] = hexValue;
	} else if(index >= 0xFF00 && index <= 0xFF7F){
		if(index == 0xFF00){ KEYS::write_keys(hexValue); }
		if(index == 0xFF40){ GPU::updateLCDcontrol(hexValue); }
		if(index == 0xFF46){ for (uint16_t i = 0; i < 0xA0; i++) { Memory::write(0xFE00 + i, Memory::read(i + (((((uint16_t) hexValue)) << 8)))) ; } }
		IO[index - 0xFF00] = hexValue;
	} else if(index >= 0xFF80 && index <= 0xFFFF){
		if(index == 0xFF80) return;
		index = index - 0xFF80;
		HRAM[index] = hexValue;
	} else {
		std::cout << "error look in memory.cpp write " << std::endl;
		//Register::printRegisters();
	}
}

std::uint8_t Memory::read(uint16_t index) {
	if(index >= 0x0000 && index <= 0x100 && !CPU::isBootDone()){
		return bootMem[index];
	} else if(index >= 0x0000 && index <= 0x7FFF){
		return cartROM[index];
	} else if(index >= 0x8000 && index <= 0x9FFF){
		return VRAM[index - 0x8000];
	} else if(index >= 0xA000 && index <=0xBFFF){
		return cartRAM[index - 0xA000];
	} else if(index >= 0xC000 && index <= 0xDFFF){
		return WRAM[index - 0xC000];
	} else if(index >= 0xE000 && index <= 0xFDFF){
		return ECHO[index - 0xE000];
	} else if(index >= 0xFE00 && index <= 0xFE9F){
		return OAM[index - 0xFE00];
	} else if (index >= 0xFEA0 && index <= 0xFEFF){
		return unusedMemory[index - 0xFEA0];
	} else if(index >= 0xFF00 && index <= 0xFF7F){
		if(index == 0xFF00){ return KEYS::read_keys(); }
                if(index == 0xFF04){ return rand();}
		return IO[index - 0xFF00];
	} else if(index >= 0xFF80 && index <= 0xFFFF){
		return HRAM[index - 0xFF80];
	} else {
		std::cout << "error look in memory.cpp read: ";
		std::cout << std::hex << (int)Register::read16($PC) << std::endl;
		return -1;
	}
	std::cout << "error look in memory.cpp read2" << std::endl;
	std::cout << std::hex << (int)Register::read16($PC) << std::endl;
	return -1;
}


void Memory::updateTileCollection(uint16_t address) {
	uint16_t tileHeadAddress = address & 0xFFF0;
	uint16_t tileindex = (address - 0x8000) >> 4;
	for (int i = 0; i <= 7; i++) {
		uint8_t mostSigByte  = read(tileHeadAddress + i * 2);
		uint8_t leastSigByte = read(tileHeadAddress + i * 2 + 1);
		for (int j = 0; j <= 7; j++) {
			uint8_t pixel = (((mostSigByte >> (7 - j)) & 0x01) << 1) | ((leastSigByte >> (7 - j)) & 0x01);
			tileCollection[tileindex][i][j] = pixel;
		}
	}
}

void Memory::printSingleTile(uint16_t tile) {
	for (int j = 0; j < 8; j++) {
		for (int k = 0; k < 8; k++)	{
			if ((int) tileCollection[tile][j][k] == 0)
				std::cout << "  ";
			else if ((int) tileCollection[tile][j][k] == 1 )
				std::cout << ". ";
			else if ((int) tileCollection[tile][j][k] == 2 )
				std::cout << "+ ";
			else if ((int) tileCollection[tile][j][k] == 3 )
				std::cout << "# ";
		}
		std::cout << std::endl;
	}
	std::cout  << std::endl;
}

void Memory::printTiles(){
	for (int i = 0; i < 384; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++)	{
				if ((int) tileCollection[i][j][k] == 0)
					std::cout << "  ";
				else if ((int) tileCollection[i][j][k] == 1 )
					std::cout << ". ";
				else if ((int) tileCollection[i][j][k] == 2 )
					std::cout << "+ ";
				else if ((int) tileCollection[i][j][k] == 3 )
					std::cout << "# ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}


uint8_t Memory::getTilePixel(int tileIndex, int x, int y) {
	return tileCollection[tileIndex][x][y];
}


void Memory::updateSpriteCollection(uint16_t address, uint8_t value) {
	uint16_t index = address >> 2;
	if (index < 0x0028){
		switch(address & 0x03){
			case(0): spriteCollection[index].ypos	 = value - 16; break;
			case(1): spriteCollection[index].xpos	 = value - 8; break;
			case(2): spriteCollection[index].tile	 = value;	  break;
			case(3):
				spriteCollection[index].priority	 	 = (value & 0x80) ? 1 : 0;
				spriteCollection[index].yflip		     = (value & 0x40) ? 1 : 0;
				spriteCollection[index].xflip		     = (value & 0x20) ? 1 : 0;
				spriteCollection[index].palette	         = (value & 0x10) ? 1 : 0;
				break;
		}
	}
}
