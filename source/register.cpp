#include "../include/register.hpp"

Register::Register(){};
Register::~Register(){};
/* begin 8bit registers */
std::uint8_t Register::$A;
std::uint8_t Register::$B;
std::uint8_t Register::$C;
std::uint8_t Register::$D;
std::uint8_t Register::$E;
std::uint8_t Register::$F;
std::uint8_t Register::$H;
std::uint8_t Register::$L;
/* begin 16bit registers */
std::uint16_t Register::$SP;
std::uint16_t Register::$PC;


void Register::resetRegisters(){ Register::write16("$PC", 0x00); }


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

bool Register::isCarry()  {	return (Register::$F & 0x10) == 0x10;}
bool Register::isHCarry() { return (Register::$F & 0x20) == 0x20;}
bool Register::isSub()    { return (Register::$F & 0x40) == 0x40;}
bool Register::isZero()   { return (Register::$F & 0x80) == 0x80;}

void Register::set_flag_carry()        { Register::write8("$F", (Register::$F  | 0x10)); }
void Register::set_flag_half_carry()   { Register::write8("$F", (Register::$F  | 0x20)); }
void Register::set_flag_sub()          { Register::write8("$F", (Register::$F  | 0x40)); }
void Register::set_flag_zero()         { Register::write8("$F", (Register::$F  | 0x80)); }

void Register::reset_flag_carry()      { Register::write8("$F", ((Register::$F | 0x10) ^ 0x10)); }
void Register::reset_flag_half_carry() { Register::write8("$F", ((Register::$F | 0x20) ^ 0x20)); }
void Register::reset_flag_sub()        { Register::write8("$F", ((Register::$F | 0x40) ^ 0x40)); }
void Register::reset_flag_zero()       { Register::write8("$F", ((Register::$F | 0x80) ^ 0x80)); }


void Register::write8(std::string registerName, std::uint8_t hexValue){
	if(registerName == "$A")      Register::$A = hexValue;
	else if(registerName == "$B") Register::$B = hexValue;
	else if(registerName == "$C") Register::$C = hexValue;
	else if(registerName == "$D") Register::$D = hexValue;
	else if(registerName == "$E") Register::$E = hexValue;
	else if(registerName == "$F") Register::$F = hexValue;
	else if(registerName == "$H") Register::$H = hexValue;
	else if(registerName == "$L") Register::$L = hexValue;
	else std::cout << "bad value in write 8 register" << std::endl;
}

std::uint8_t Register::read8(std::string registerName){
	if(registerName == "$A")       return Register::$A;
	else if(registerName == "$B")  return Register::$B;
	else if(registerName == "$C")  return Register::$C;
	else if(registerName == "$D")  return Register::$D;
	else if(registerName == "$E")  return Register::$E;
	else if(registerName == "$F")  return Register::$F;
	else if(registerName == "$H")  return Register::$H;
	else if(registerName == "$L")  return Register::$L;
	else {
		std::cout << "Watch register input name 8 bit " << registerName << std::endl;
		return -1; //ERROR, BAD INPUT
	}
}

void Register::write16(std::string registerName, std::uint16_t hexValue){
	if(registerName == "$AF"){
		Register::$A  = hexValue >> 8;
		Register::$F  = hexValue & 0xFF;
	} else if(registerName == "$BC"){
		Register::$B  = hexValue >> 8;
		Register::$C  = hexValue & 0xFF;
	} else if(registerName == "$DE"){
		Register::$D  = hexValue >> 8;
		Register::$E  = hexValue & 0xFF;
	} else if(registerName == "$HL"){
		Register::$H  = hexValue >> 8;
		Register::$L  = hexValue & 0xFF;
	} else if(registerName == "$SP"){
		Register::$SP = hexValue;
	} else if(registerName == "$PC"){
		Register::$PC = hexValue;
	}
}

std::uint16_t Register::read16(std::string registerName){
	if(registerName == "$AF")       return Register::$A << 8 | Register::$F;
	else if(registerName == "$BC")  return Register::$B << 8 | Register::$C;
	else if(registerName == "$DE")  return Register::$D << 8 | Register::$E;
	else if(registerName == "$HL")  return Register::$H << 8 | Register::$L;
	else if(registerName == "$SP")  return Register::$SP;
	else if(registerName == "$PC")  return Register::$PC;
	else{
		std::cout << "Watch register input name 16 bit" << registerName << std::endl;
		return -1; //ERROR, BAD INPUT
	}
}
