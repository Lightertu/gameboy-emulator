/*
Written by: Haley Whitman
			Trace Andreason
			Andrew Hill
			Rui Tu
*/

#include <iostream>
#include <fstream>
#include <string>

#include "./include/isa.hpp"
#include "./include/loader.hpp"
#include "./include/register.hpp"
#include "./include/cpu.hpp"
#include "./include/test.hpp"
#include "./include/memory.hpp"
#include "./include/alu.hpp"
#include "./include/databank.hpp"
#include <ctime>
#include <time.h>

using namespace std;

/*
CITATIONS:
SDL framework draws heavily from Imrannazar's javascript gameboy emulator blog found here:
http://imrannazar.com/GameBoy-Emulation-in-JavaScript

Overall instruction set follows from Cturt's Cinoop gameboy emulator written in C found here:
https://cturt.github.io/cinoop.html
*/


/*
TO RUN:
$ make
% ./main
*/
int main(){
	char a, b;
	ifstream inputROM;
	inputROM.open("./ROMFiles/Tetris.txt");
	int i = 0;
	Memory::cleanMemory();
	while(inputROM >> a >> b and i != 32770){ //getting two char's from a text file.
		loader(a, b, i);
		i++;
	}

	//if(!GPU::initializeWindow()){}
	Register::resetRegisters();
	Memory::initializeMemory();
        srand(time(NULL));

	//waitSeconds(5);

  int success = initializeWindow();

	cout << "Begin Bootstrap" << endl;
	CPU::bootstrap();
	std::cout << "Begin Gameboy Game" << std::endl;
	CPU::run();
	//Memory::printTiles();
	//Memory::printMemory(0xFF00, 0xFFFF);
 	return 0;
}

