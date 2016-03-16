#ifndef LOADER_H_
#define LOADER_H_

#include <iostream>
#include <fstream>
#include <cstdint>
#include <map>
#include "memory.hpp"


//Opcode is what is going to be inserted into memory, TODO change variables.
class Opcode {
public:
	std::uint8_t opcode;
	std::uint8_t argument1;
	std::uint8_t argument2;
};

void loader(char a, char b, int i);

#endif
