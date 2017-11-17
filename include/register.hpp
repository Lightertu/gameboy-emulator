#ifndef REGISTER_H_
#define REGISTER_H_

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "../include/memory.hpp"

/* We will figure out later if this is the correct implementation or not.
	This is all hexadecimal besides the shorts. We could just make it all
	shorts and then limit the size another way. Or if we need to even limit
	the size. std::stoi(hexValue, nullptr, 16)
*/
class Register {
public:
	Register();
	virtual ~Register();

	static void resetRegisters();
  	static void write8(int registerName, std::uint8_t hexValue);
  	static std::uint8_t read8(int registerName);
  	static void write16(int registerName, std::uint16_t hexValue);
  	static std::uint16_t read16(int registerName);
	static bool isCarry();
	static bool isHCarry();
	static bool isZero();
	static bool isSub();
	static void set_flag_zero();
	static void set_flag_sub();
	static void set_flag_half_carry();
	static void set_flag_carry();

	/* reset flag operation */
	static void reset_flag_zero();
	static void reset_flag_sub();
	static void reset_flag_half_carry();
	static void reset_flag_carry();

  //static void printRegisters();
};



#endif
