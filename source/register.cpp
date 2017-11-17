#include "../include/register.hpp"

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

Register::Register(){};
Register::~Register(){};

std::uint8_t Registers[12];
std::uint16_t *Registers16 = (std::uint16_t *) Registers;


void Register::resetRegisters(){ Register::write16($PC, 0x00); }

/*
void Register::printRegisters(){
// 	std::cout << "Preforming Instruction: " << std::hex << (int)Memory::read(Register::read16("$PC")) << std::endl;
	std::cout << "$AF: " << std::hex << (int)Register::read16("$AF") << std::endl;
	std::cout << "$BC: " << std::hex << (int)Register::read16("$BC") << std::endl;
	std::cout << "$DE: " << std::hex << (int)Register::read16("$DE") << std::endl;
	std::cout << "$HL: " << std::hex << (int)Register::read16("$HL") << std::endl;
	std::cout << "$SP: " << std::hex << (int)Register::read16("$SP") << std::endl;
	std::cout << "$PC: " << std::hex << (int)Register::read16("$PC") << std::endl;
	std::cout << "Z: " << (int)Register::isZero() << " ";
	std::cout << "N: " << (int)Register::isSub() << " ";
	std::cout << "H: " << (int)Register::isHCarry() << " ";
	std::cout << "C: " << (int)Register::isCarry() << " " << std::endl;;
 	std::cout << "******************************************************************" << std::endl;
}
*/

bool Register::isCarry()  { return Registers[$F] & 0x10;}
bool Register::isHCarry() { return Registers[$F] & 0x20;}
bool Register::isSub()    { return Registers[$F] & 0x40;}
bool Register::isZero()   { return Registers[$F] & 0x80;}

void Register::set_flag_carry()        { Registers[$F]  |= 0x10; }
void Register::set_flag_half_carry()   { Registers[$F]  |= 0x20; }
void Register::set_flag_sub()          { Registers[$F]  |= 0x40; }
void Register::set_flag_zero()         { Registers[$F]  |= 0x80; }

void Register::reset_flag_carry()      { Register::write8($F, ((Registers[$F] | 0x10) ^ 0x10)); }
void Register::reset_flag_half_carry() { Register::write8($F, ((Registers[$F] | 0x20) ^ 0x20)); }
void Register::reset_flag_sub()        { Register::write8($F, ((Registers[$F] | 0x40) ^ 0x40)); }
void Register::reset_flag_zero()       { Register::write8($F, ((Registers[$F] | 0x80) ^ 0x80)); }


void Register::write8(int registerName, std::uint8_t hexValue){
    Registers[registerName] = hexValue;
}

std::uint8_t Register::read8(int registerName){
    return Registers[registerName];
}

void Register::write16(int registerName, std::uint16_t hexValue){
    Registers16[registerName] = hexValue;
}

std::uint16_t Register::read16(int registerName){
    return Registers16[registerName];
}
