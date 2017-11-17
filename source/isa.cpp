#include "../include/isa.hpp"
#include "../include/display.hpp"
#include <SDL2/SDL.h>

ISA::ISA(){};
ISA::~ISA(){};
extern SDL_Renderer *gRenderer;

#define write8 Register::write8
#define write16 Register::write16
#define read8 Register::read8
#define read16 Register::read16
#define add_8 ALU::add_8
#define adc_8 ALU::adc_8
#define sub_8 ALU::sub_8
#define sbc_8 ALU::sbc_8
#define and_8 ALU::and_8
#define readMem Memory::read
#define writeMem Memory::write
#define xor_8 ALU::xor_8
#define or_8 ALU::or_8
#define cp_8 ALU::cp_8
#define inc_8 ALU::inc_8
#define dec_8 ALU::dec_8
#define inc_16 ALU::inc_16
#define dec_16 ALU::dec_16
#define add_hl ALU::add_hl
#define add_sp ALU::add_sp
#define bit ALU::bit
#define set ALU::set
#define res ALU::reset
#define rotate_left_through_carry ALU::rotate_left_through_carry
#define rotate_left_to_carry ALU::rotate_left_to_carry
#define rotate_right_through_carry ALU::rotate_right_through_carry
#define rotate_right_to_carry ALU::rotate_right_to_carry
#define shift_left_to_carry ALU::shift_left_to_carry
#define arithmetic_shift_right_to_carry ALU::arithmetic_shift_right_to_carry
#define logic_shift_right_to_carry ALU::logic_shift_right_to_carry
#define swap ALU::swap

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


/*
CITATIONS
Overall instruction set follows from Cturt's Cinoop gameboy emulator written in C found here:
https://cturt.github.io/cinoop.html

All function headers are from the official gameboy manual.
*/

int ISA::master_enable = 1;

/* ************************************** 8-BIT LOADS ************************************** */
/* ******************* LD nn,n ******************* */
// Description:
// Put value nn into n.
// Use with:
// nn = B,C,D,E,H,L,BC,DE,HL,SP n = 8 bit immediate value
void ISA::ld_b_n(std::uint8_t operand){write8($B, operand);}
void ISA::ld_c_n(std::uint8_t operand){write8($C, operand);}
void ISA::ld_d_n(std::uint8_t operand){write8($D, operand);}
void ISA::ld_e_n(std::uint8_t operand){write8($E, operand);}
void ISA::ld_h_n(std::uint8_t operand){write8($H, operand);}
void ISA::ld_l_n(std::uint8_t operand){write8($L, operand);}

/* ******************* LD r1,r2 ******************* */
// Description:
//  Put value r2 into r1.
// Use with:
//  r1,r2 = A,B,C,D,E,H,L,(HL)
//void ISA::ld_b_b()  {write8($B, read8($B));}
void ISA::ld_b_c()  {write8($B, read8($C));}
void ISA::ld_b_d()  {write8($B, read8($D));}
void ISA::ld_b_e()  {write8($B, read8($E));}
void ISA::ld_b_h()  {write8($B, read8($H));}
void ISA::ld_b_l()  {write8($B, read8($L));}
void ISA::ld_b_hlp(){write8($B, readMem(read16($HL)));}

void ISA::ld_c_b(){write8($C, read8($B));}
//TODO Missing ld_c_c
void ISA::ld_c_d(){write8($C, read8($D));}
void ISA::ld_c_e(){write8($C, read8($E));}
void ISA::ld_c_h(){write8($C, read8($H));}
void ISA::ld_c_l(){write8($C, read8($L));}
void ISA::ld_c_hlp(){write8($C, readMem(read16($HL)));}

void ISA::ld_d_b(){write8($D, read8($B));}
void ISA::ld_d_c(){write8($D, read8($C));}
//TODO mising ld_d_d
void ISA::ld_d_e(){write8($D, read8($E));}
void ISA::ld_d_h(){write8($D, read8($H));}
void ISA::ld_d_l(){write8($D, read8($L));}
void ISA::ld_d_hlp(){write8($D, readMem(read16($HL)));}

void ISA::ld_e_b(){write8($E, read8($B));}
void ISA::ld_e_c(){write8($E, read8($C));}
void ISA::ld_e_d(){write8($E, read8($D));}
//TODO missing ld_e_e
void ISA::ld_e_h(){write8($E, read8($H));}
void ISA::ld_e_l(){write8($E, read8($L));}
void ISA::ld_e_hlp(){write8($E, readMem(read16($HL)));}

void ISA::ld_h_b(){write8($H, read8($B));}
void ISA::ld_h_c(){write8($H, read8($C));}
void ISA::ld_h_d(){write8($H, read8($D));}
void ISA::ld_h_e(){write8($H, read8($E));}
//TODO missing ld_h_h
void ISA::ld_h_l(){write8($H, read8($L));}
void ISA::ld_h_hlp(){write8($H, readMem(read16($HL)));}

void ISA::ld_l_b(){write8($L, read8($B));}
void ISA::ld_l_c(){write8($L, read8($C));}
void ISA::ld_l_d(){write8($L, read8($D));}
void ISA::ld_l_e(){write8($L, read8($E));}
void ISA::ld_l_h(){write8($L, read8($H));}
//TODO missing ld_l_l
void ISA::ld_l_hlp(){write8($L, readMem(read16($HL)));}

void ISA::ld_hlp_b(){writeMem(read16($HL), read8($B));}
void ISA::ld_hlp_c(){writeMem(read16($HL), read8($C));}
void ISA::ld_hlp_d(){writeMem(read16($HL), read8($D));}
void ISA::ld_hlp_e(){writeMem(read16($HL), read8($B));}
void ISA::ld_hlp_h(){writeMem(read16($HL), read8($H));}
void ISA::ld_hlp_l(){writeMem(read16($HL), read8($L));}
// NOTE: last bug NO.1
void ISA::ld_hlp_n(uint8_t operand){writeMem(read16($HL), operand);}

/* ******************* LD A,n ******************* */
// Description:
//  Put value n into A.
// Use with:
//  n = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn),#
//  nn = two byte immediate value. (LS byte first.)

void ISA::ld_a_b(){write8($A, read8($B));}
void ISA::ld_a_c(){write8($A, read8($C));}
void ISA::ld_a_d(){write8($A, read8($D));}
void ISA::ld_a_e(){write8($A, read8($E));}
void ISA::ld_a_h(){write8($A, read8($H));}
void ISA::ld_a_l(){write8($A, read8($L));}

void ISA::ld_a_n(uint8_t operand){write8($A, operand);}
//NOTE: swapped bytes
void ISA::ld_a_nnp(uint16_t operand){write8($A, readMem(operand));}

void ISA::ld_a_dep(){write8($A, readMem(read16($DE)));}
void ISA::ld_a_bcp(){write8($A, readMem(read16($BC)));}
void ISA::ld_a_hlp(){write8($A, readMem(read16($HL)));}

/* ******************* LD n,A ******************* */
// Description:
//  Put value A into n.
// Use with:
//  n = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn)
//  nn = two byte immediate value. (LS byte first.)

//TODO missing ld_a_a
void ISA::ld_b_a(){write8($B, read8($A));}
void ISA::ld_c_a(){write8($C, read8($A));}
void ISA::ld_d_a(){write8($D, read8($A));}
void ISA::ld_e_a(){write8($E, read8($A));}
void ISA::ld_h_a(){write8($H, read8($A));}
void ISA::ld_l_a(){write8($L, read8($A));}

void ISA::ld_bcp_a(){writeMem(read16($BC), read8($A));}
void ISA::ld_dep_a(){writeMem(read16($DE), read8($A));}
void ISA::ld_hlp_a(){writeMem(read16($HL), read8($A));}
//NOTE: swapped bytes
void ISA::ld_nnp_a(std::uint16_t operand){ writeMem(operand, read8($A));}
/* ******************* LD A,(C) ******************* */
// Description:
//  Put value at address $FF00 + register C into A. Same as: LD A,($FF00+C)

void ISA::ld_a_ff_c(){write8($A, (readMem(0xFF00 + read8($C))) );}

/* ******************* LD (C),A ******************* */
// Description:
//  Put A into address $FF00 + register C.

void ISA::ld_ff_c_a(){writeMem((0xFF00 + read8($C)), read8($A));}

/* ******************* LD A,(HL) ******************* */
// Description:
//  Put value at address HL into A. Decrement HL. Same as: LD A,(HL) - DEC HL
void ISA::ldd_a_hlp(){
    write8($A, readMem(read16($HL)));
    write16($HL, dec_16(read16($HL)));
}
/* ******************* LD (HL),A ******************* */
// Description:
//  Put A into memory address HL. Decrement HL. Same as: LD (HL),A - DEC HL
void ISA::ldd_hlp_a(){
  writeMem(read16($HL), read8($A));
  write16($HL, dec_16(read16($HL)));
}
/* ******************* LD A,(HL) ******************* */
// Description:
//  Put value at address HL into A. Increment HL. Same as: LD A,(HL) - INC HL
void ISA::ldi_a_hlp(){
    write8($A, readMem(read16($HL)));
    write16($HL, inc_16(read16($HL)));
}
/* ******************* LD (HL),A ******************* */
// Description:
//  Put A into memory address HL. Increment HL. Same as: LD (HL),A - INC HL
void ISA::ldi_hlp_a(){
  //std::cout << "in func" << std::endl;
    writeMem(read16($HL), read8($A));
    //std::cout << "wrote mem" << std::endl;
    write16($HL, inc_16(read16($HL)));
}
/* ******************* LDH (n),A ******************* */
// Description:
//  Put A into memory address $FF00+n.
// Use with:
//  n = one byte immediate value.
void ISA::ld_ff_n_ap(uint8_t operand){
  writeMem((0xFF00 + operand), read8($A));
}
/* ******************* LDH A,(n) ******************* */ //TODO Read memory or no
// Description:
//  Put memory address $FF00+n into A.
// Use with:
//  n = one byte immediate value.
void ISA::ld_ff_ap_n(uint8_t operand){
  write8($A, readMem(0xFF00 + operand));
}

//NOTE:potential bug
// void ISA::ld_ff_ap_n(uint8_t operand){
//   write8($A, (0xFF00 + operand));
// }

/* ************************************** 16-BIT LOADS ************************************** */

/* ******************* LD n,nn ******************* */
// Description:
//  Put value nn into n.
// Use with:
//  n = BC,DE,HL,SP
//  nn = 16 bit immediate value
void ISA::ld_bc_nn(std::uint16_t operand){write16($BC, operand);}
void ISA::ld_de_nn(std::uint16_t operand){write16($DE, operand);}
void ISA::ld_hl_nn(std::uint16_t operand){write16($HL, operand);}
void ISA::ld_sp_nn(std::uint16_t operand){write16($SP, operand);}

/* ******************* LD SP,HL ******************* */
// Description:
//  Put HL into Stack Pointer (SP).
void ISA::ld_sp_hl(){write16($SP, read16($HL));}

/* ******************* LD SP,n ******************* */
// Description:
//  Put SP + n effective address into HL.
// Use with:
//  n = one byte signed immediate value.
// Flags affected:
//  Z - Reset.
//  N - Reset.
//  H - Set or reset according to operation. C - Set or reset according to operation.
void ISA::ld_hl_sp_n(uint8_t operand){ write16($HL, add_sp( (int8_t) operand) );}

/* ******************* LD (nn),SP ******************* */
// Description:
//  Put Stack Pointer (SP) at address n.
// Use with:
//  nn = two byte immediate address.
void ISA::ld_nnp_sp(std::uint16_t operand){
    writeMem(operand, (uint8_t)(read16($SP) & 0x00ff));
	writeMem(operand + 1, (uint8_t)((read16($SP) & 0xff00) >> 8));
}

/* ******************* PUSH nn ******************* */
// Description:
//  Push register pair nn onto stack.
//  Decrement Stack Pointer (SP) twice.
// Use with:
//  nn = AF,BC,DE,HL
void ISA::push_af(){push(read16($AF));}
void ISA::push_bc(){push(read16($BC));}
void ISA::push_de(){push(read16($DE));}
void ISA::push_hl(){push(read16($HL));}

void ISA::push(uint16_t pairReg) {
  uint8_t firstByte  = pairReg >> 8;
  uint8_t secondByte = pairReg & 0xFF;
  //std::cout<<"SP push was: "<< std::hex<<(int)Register::read16($PC)<<std::endl;
  //std::cout<<"SP push was: "<< std::hex<<(int)Register::read16($SP)<<std::endl;
  //std::cout<<"SP push was: "<< std::hex<<(int)Memory::read(Register::read16($SP))<<std::endl;
  Memory::write(Register::read16($SP), firstByte);
  Register::write16($SP,dec_16(Register::read16($SP)));

  Memory::write( Register::read16($SP), secondByte);
  Register::write16($SP,dec_16(Register::read16($SP)));
  //std::cout<<"SP push was: "<< std::hex<<(int)Register::read16($SP)<<std::endl;
  //std::cout<<"SP push now: "<< std::hex<<(int)Memory::read(Register::read16($SP))<<std::endl;
}

/* ******************* POP nn ******************* */
// Description:
//  Pop two bytes off stack into register pair nn. Increment Stack Pointer (SP) twice.
// Use with:
//  nn = AF,BC,DE,HL
void ISA::pop_af(){write16($AF,(pop()));}
void ISA::pop_bc(){write16($BC,(pop()));}
void ISA::pop_de(){write16($DE,(pop()));}
void ISA::pop_hl(){write16($HL,(pop()));}

uint16_t ISA::pop() {
  //std::cout<<"SP pop was: "<< std::hex<<(int)Register::read16($SP)<<std::endl;
  //std::cout<<"SP pop was: "<< std::hex<<(int)Memory::read(Register::read16($SP))<<std::endl;
  Register::write16($SP,inc_16(Register::read16($SP)));
  uint8_t firstByte = Memory::read(Register::read16($SP));
  Memory::write(Register::read16($SP), 0);

  Register::write16($SP,inc_16(Register::read16($SP)));
  uint8_t secondByte = Memory::read(Register::read16($SP));
  Memory::write(Register::read16($SP), 0);
  //std::cout<<"SP pop was: "<< std::hex<<(int)Register::read16($SP)<<std::endl;
  //std::cout<<"SP pop now: "<< std::hex<<(int)Memory::read(Register::read16($SP))<<std::endl;
  return secondByte << 8 | firstByte;
}

/* ************************************** MISCELLANEOUS ************************************** */

/* ******************* SWAP n ******************* */
// Description:
//  Swap upper & lower nibles of n.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Reset.
void ISA::swap_a(){write8($A, swap(read8($A)));}
void ISA::swap_b(){write8($B, swap(read8($B)));}
void ISA::swap_c(){write8($C, swap(read8($C)));}
void ISA::swap_d(){write8($D, swap(read8($D)));}
void ISA::swap_e(){write8($E, swap(read8($E)));}
void ISA::swap_h(){write8($H, swap(read8($H)));}
void ISA::swap_l(){write8($L, swap(read8($L)));}
void ISA::swap_hlp(){writeMem(read16($HL), swap(readMem(read16($HL))));}
/* ******************* DAA ******************* */
// Description:
//  Decimal adjust register A.
//  This instruction adjusts register A so that the correct representation of Binary Coded Decimal (BCD) is obtained.
// Flags affected:
//  Z - Set if register A is zero.
//  N - Not affected.
//  H - Reset.
//  C - Set or reset according to operation.
void ISA::daa(){
  //Shouldnt have to implement, very rare instruction.
    if(Register::isSub()) {
      if(Register::isHCarry()) Register::write8($A, (Register::read8($A) - 0x06) & 0xFF);
      if(Register::isCarry()) Register::write8($A, (Register::read8($A) - 0x60));
    }
    else {
      if(Register::isHCarry() || (Register::read8($A) & 0x0F) > 9)
        Register::write8($A, (Register::read8($A) + 0x06));
      if(Register::isCarry() || (Register::read8($A) > 0x9F))
        Register::write8($A, (Register::read8($A) + 0x60));;
    }

    Register::reset_flag_half_carry();

    if(Register::read8($A)) Register::reset_flag_zero();
    else Register::reset_flag_zero();

    if(Register::read8($A) >= 0x100) Register::set_flag_carry();
}
/* ******************* CPL ******************* */
// Description:
//  Complement A register. (Flip all bits.)
void ISA::cpl(){
  write8($A,~read8($A)); /*FLAGS_SET(FLAGS_NEGATIVE | FLAGS_HALFCARRY);*/
  Register::set_flag_half_carry();
  Register::set_flag_sub();
}
/* ******************* CCF ******************* */
// Description:
//  Complement carry flag.
//  If C flag is set, then reset it. If C flag is reset, then set it.
void ISA::ccf(){
  if(Register::isCarry()) Register::reset_flag_carry();
  else Register::set_flag_carry();
  Register::reset_flag_half_carry();
  Register::reset_flag_sub();
}

/* ******************* SCF ******************* */
// Description:
//  Set Carry flag.
// Flags affected:
//  Z - Not affected. N - Reset.
//  H - Reset.
//  C - Set.
void ISA::scf(){
  Register::set_flag_carry();
  Register::reset_flag_half_carry();
  Register::reset_flag_sub();
}

/* ******************* NOP ******************* */
// Description:
//   No operation.
void ISA::nop(){}

/* ******************* HALT ******************* */
// Description:
//  Power down CPU until an interrupt occurs.
//  Use this when ever possible to reduce energy consumption.
//TODO
void ISA::halt(){
  if(ISA::master_enable) {
    write16($PC, (read16($PC) - 0x01));
    //HALT EXECUTION UNTIL AN INTERRUPT OCCURS
  }else{
    //
  }
}

/* ******************* STOP ******************* */ //TODO
// Description:
//  Halt CPU & LCD display until button pressed.
void ISA::stop(uint8_t operand){
  //std::cout<<"stop";
  /*
  HALT's the CPU until an interrupt occurrs. During HALT,
  the CPU power consumption is reduced. It's generally pretty smart to place
  two NOP's after a HALT due to a hardware bug.
  */

}
/* ******************* DI ******************* */
// Description:
//  This instruction disables interrupts but not immediately.
//  Interrupts are disabled after instruction after DI is executed.
void ISA::di_inst(){
  //std::cout << "DI" << std::endl;
  master_enable = 0;
}
/* ******************* EI ******************* */
// Description:
//  Enable interrupts. This intruction enables interrupts but not immediately.
//  Interrupts are enabled after instruction after EI is executed.
 void ISA::ei(){
   //std::cout << "EI" << std::endl;
   master_enable = 1;
 }

 /* ************************************** JUMPS ************************************** */

 /* ******************* JP nn ******************* */
// Description:
//  Jump to address nn.
// Use with:
//  nn = two byte immediate value. (LS byte first.)
//NOTE: swapped bytes
void ISA::jp_nn(uint16_t operand){ write16($PC, (operand)); }
/* ******************* JP cc,nn ******************* */
// Description:
//  Jump to address n if following condition is true:
//    cc = NZ, Jump if Z flag is reset.
//    cc = Z, Jump if Z flag is set.
//    cc = NC, Jump if C flag is reset.
//    cc = C, Jump if C flag is set.
void ISA::jp_nz_nn(uint16_t operand){
  if(!(Register::isZero())){
    jp_nn(operand);
    CPU::incrClockBy(16);
    }else CPU::incrClockBy(12);
}
void ISA::jp_z_nn(uint16_t operand){
  if(Register::isZero()){
    jp_nn(operand);
    CPU::incrClockBy(16);
  }else CPU::incrClockBy(12);
}
void ISA::jp_nc_nn(uint16_t operand){
  if(!Register::isCarry()){
    jp_nn(operand);
    CPU::incrClockBy(16);
  }else CPU::incrClockBy(12);
}
void ISA::jp_c_nn(uint16_t operand){
  if(Register::isCarry()){
    jp_nn(operand);
    CPU::incrClockBy(16);
  }else CPU::incrClockBy(12);
}
/* ******************* JP (HL) ******************* */
// Description:
//  Jump to address contained in HL.
void ISA::jp_hl(){
  write16($PC, read16($HL));
}
/* ******************* JR n ******************* */
// Description:
//  Add n to current address and jump to it.
// Use with:
//  n = one byte signed immediate value
void ISA::jr_n(std::uint8_t operand){write16($PC, (read16($PC) + (int8_t)operand));}
/* ******************* JR cc,n ******************* */
// Description:
//  If following condition is true then add n to current address and jump to it:
// Use with:
//  n = one byte signed immediate value
//    cc = NZ, Jump if Z flag is reset.
//    cc = Z, Jump if Z flag is set.
//    cc = NC, Jump if C flag is reset.
//    cc = C, Jump if C flag is set.
void ISA::jr_nz_n(uint8_t operand){
  if(Register::isZero())
    CPU::incrClockBy(8);
  else{
    CPU::incrClockBy(12);
    write16($PC, (read16($PC) + (int8_t)operand));
  }
}
void ISA::jr_z_n(uint8_t operand){
  if(Register::isZero()){
    write16($PC, (read16($PC) + (int8_t)operand));
    CPU::incrClockBy(12);
  } else CPU::incrClockBy(8);
}
void ISA::jr_nc_n(uint8_t operand){
  if(Register::isCarry())
    CPU::incrClockBy(8);
  else{
    write16($PC, (read16($PC) + (int8_t)operand));
    CPU::incrClockBy(12);
  }
}
void ISA::jr_c_n(uint8_t operand){
  if(Register::isCarry()){
    write16($PC, (read16($PC) + (int8_t)operand));
    CPU::incrClockBy(12);
  } else CPU::incrClockBy(8);
}

/***************************8 bit Arithmetic*******************************/

/* ******************* ADD A,n ******************* */
// Description:
//  Add n to A.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Set if carry from bit 3.
//  C - Set if carry from bit 7.
void ISA::add_a_a()  {write8($A,add_8(read8($A)));}
void ISA::add_a_b()  {write8($A,add_8(read8($B)));}
void ISA::add_a_c()  {write8($A,add_8(read8($C)));}
void ISA::add_a_d()  {write8($A,add_8(read8($D)));}
void ISA::add_a_e()  {write8($A,add_8(read8($E)));}
void ISA::add_a_h()  {write8($A,add_8(read8($H)));}
void ISA::add_a_l()  {write8($A,add_8(read8($L)));}
void ISA::add_a_hlp(){write8($A,add_8(readMem(read16($HL))));}
void ISA::add_a_n(uint8_t operand){write8($A,add_8(operand));}
/* ******************* ADC A,n ******************* */
// Description:
//  Add n + Carry flag to A.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Set if carry from bit 3.
//  C - Set if carry from bit 7.
void ISA::adc_a()  {write8($A,adc_8(read8($A)));}
void ISA::adc_b()  {write8($A,adc_8(read8($B)));}
void ISA::adc_c()  {write8($A,adc_8(read8($C)));}
void ISA::adc_d()  {write8($A,adc_8(read8($D)));}
void ISA::adc_e()  {write8($A,adc_8(read8($E)));}
void ISA::adc_h()  {write8($A,adc_8(read8($H)));}
void ISA::adc_l()  {write8($A,adc_8(read8($L)));}
void ISA::adc_hlp(){write8($A,adc_8(readMem(read16($HL))));}
void ISA::adc_n(uint8_t operand){write8($A,adc_8(operand));}
/* ******************* SUB A,n ******************* */
// Description:
//  Subtract n from A.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero.
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Set if no borrow.
void ISA::sub_a(){write8($A,sub_8(read8($A)));}
void ISA::sub_b(){write8($A,sub_8(read8($B)));}
void ISA::sub_c(){write8($A,sub_8(read8($C)));}
void ISA::sub_d(){write8($A,sub_8(read8($D)));}
void ISA::sub_e(){write8($A,sub_8(read8($E)));}
void ISA::sub_h(){write8($A,sub_8(read8($H)));}
void ISA::sub_l(){write8($A,sub_8(read8($L)));}
void ISA::sub_hlp(){write8($A,sub_8(readMem(read16($HL))));}
void ISA::sub_n(uint8_t operand){write8($A,sub_8(operand));}
/* ******************* SBC A,n ******************* */
// Description:
//  Subtract n + Carry flag from A.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero.
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Set if no borrow.
void ISA::sbc_a(){write8($A,(sbc_8(read8($A))));}
void ISA::sbc_b(){write8($A,(sbc_8(read8($B))));}
void ISA::sbc_c(){write8($A,(sbc_8(read8($C))));}
void ISA::sbc_d(){write8($A,(sbc_8(read8($D))));}
void ISA::sbc_e(){write8($A,(sbc_8(read8($E))));}
void ISA::sbc_h(){write8($A,(sbc_8(read8($H))));}
void ISA::sbc_l(){write8($A,(sbc_8(read8($L))));}
void ISA::sbc_hlp(){write8($A,(sbc_8(readMem(read16($HL)))));}
void ISA::sbc_n(uint8_t operand){write8($A,(sbc_8(operand)));}
/* ******************* AND n ******************* */
// Description:
//  Logically AND n with A, result in A.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Set.
//  C - Reset.
void ISA::and_a()  {write8($A,(and_8(read8($A))));}
void ISA::and_b()  {write8($A,(and_8(read8($B))));}
void ISA::and_c()  {write8($A,(and_8(read8($C))));}
void ISA::and_d()  {write8($A,(and_8(read8($D))));}
void ISA::and_e()  {write8($A,(and_8(read8($E))));}
void ISA::and_h()  {write8($A,(and_8(read8($H))));}
void ISA::and_l()  {write8($A,(and_8(read8($L))));}
void ISA::and_hlp(){write8($A,(and_8(readMem(read16($HL)))));}
void ISA::and_n(uint8_t operand){write8($A,(and_8(operand)));}
/* ******************* OR n ******************* */
// Description:
//  Logical OR n with register A, result in A.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Reset.
void ISA::or_a(){write8($A,(or_8(read8($A))));}
void ISA::or_b(){write8($A,(or_8(read8($B))));}
void ISA::or_c(){write8($A,(or_8(read8($C))));}
void ISA::or_d(){write8($A,(or_8(read8($D))));}
void ISA::or_e(){write8($A,(or_8(read8($E))));}
void ISA::or_h(){write8($A,(or_8(read8($H))));}
void ISA::or_l(){write8($A,(or_8(read8($L))));}
void ISA::or_hlp(){write8($A,(or_8(readMem(read16($HL)))));}
void ISA::or_n(uint8_t operand){ write8($A, or_8(operand));}
/* ******************* XOR n ******************* */
// Description:
//  Logical exclusive OR n with register A, result in A.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Reset.
void ISA::xor_a(){write8($A,(xor_8(read8($A))));}
void ISA::xor_b(){write8($A,(xor_8(read8($B))));}
void ISA::xor_c(){write8($A,(xor_8(read8($C))));}
void ISA::xor_d(){write8($A,(xor_8(read8($D))));}
void ISA::xor_e(){write8($A,(xor_8(read8($E))));}
void ISA::xor_h(){write8($A,(xor_8(read8($H))));}
void ISA::xor_l(){write8($A,(xor_8(read8($L))));}
void ISA::xor_hlp(){write8($A,(xor_8(readMem(read16($HL)))));}
void ISA::xor_n(uint8_t operand){write8($A,(xor_8(operand)));}
/* ******************* CP n ******************* */
// Description:
//  Compare A with n. This is basically an A - n
//  subtraction instruction but the results are thrown
//  away.
// Use with:
//  n = A,B,C,D,E,H,L,(HL),#
// Flags affected:
//  Z - Set if result is zero. (Set if A = n.)
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Set for no borrow. (Set if A < n.)
void ISA::cp_a(){cp_8(read8($B));}
void ISA::cp_b(){cp_8(read8($B));}
void ISA::cp_c(){cp_8(read8($C));}
void ISA::cp_d(){cp_8(read8($D));}
void ISA::cp_e(){cp_8(read8($E));}
void ISA::cp_h(){cp_8(read8($H));}
void ISA::cp_l(){cp_8(read8($L));}
void ISA::cp_hlp(){cp_8(readMem(read16($HL)));}
void ISA::cp_n(uint8_t operand){ cp_8(operand) ;}
/* ******************* INC n ******************* */
// Description:
//  Increment register n.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Set if carry from bit 3.
//  C - Not affected.
void ISA::inc_a(){write8($A, inc_8(read8($A)));}
void ISA::inc_b(){write8($B, inc_8(read8($B)));}
void ISA::inc_c(){write8($C, inc_8(read8($C)));}
void ISA::inc_d(){write8($D, inc_8(read8($D)));}
void ISA::inc_e(){write8($E, inc_8(read8($E)));}
void ISA::inc_h(){write8($H, inc_8(read8($H)));}
void ISA::inc_l(){write8($L, inc_8(read8($L)));}
void ISA::inc_hlp(){writeMem(read16($HL), inc_8(readMem(read16($HL))));}
/* ******************* DEC n ******************* */
// Description:
//  Decrement register n.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Not affected.
void ISA::dec_a(){write8($A, dec_8(read8($A)));}
void ISA::dec_b(){write8($B, dec_8(read8($B)));}
void ISA::dec_c(){write8($C, dec_8(read8($C)));}
void ISA::dec_d(){write8($D, dec_8(read8($D)));}
void ISA::dec_e(){write8($E, dec_8(read8($E)));}
void ISA::dec_h(){write8($H, dec_8(read8($H)));}
void ISA::dec_l(){write8($L, dec_8(read8($L)));}
void ISA::dec_hlp(){writeMem(read16($HL), dec_8(readMem(read16($HL))));}

/*************************** 16 bit Arithmetic *******************************/

/* ******************* ADD HL,n ******************* */
// Description:
//  Add n to HL.
// Use with:
//  n = BC,DE,HL,SP
// Flags affected:
//  Z - Not affected.
//  N - Reset.
//  H - Set if carry from bit 11.
//  C - Set if carry from bit 15.
void ISA::add_hl_bc(){write16($HL, add_hl(read16($BC)));}
void ISA::add_hl_de(){write16($HL, add_hl(read16($DE)));}
void ISA::add_hl_hl(){add_hl(read16($HL));}
void ISA::add_hl_sp(){write16($HL, add_hl(read16($SP)));}
/* ******************* ADD SP,n ******************* */
// Description:
//  Add n to Stack Pointer (SP).
// Use with:
//  n = one byte signed immediate value (#).
// Flags affected:
//  Z - Reset.
//  N - Reset.
//  H - Set or reset according to operation.
//  C - Set or reset according to operation.
void ISA::add_sp_n(uint8_t operand){write16($SP, add_sp( (int8_t) operand) );}
/* ******************* INC nn ******************* */
// Description:
//  Increment register nn.
//
// Use with:
//  nn = BC,DE,HL,SP
//
// Flags affected:
//  None.
void ISA::inc_bc(){write16($BC, inc_16(read16($BC)));}
void ISA::inc_de(){write16($DE, inc_16(read16($DE)));}

//NOTE:change
void ISA::inc_hl(){write16($HL, inc_16(read16($HL)));}
void ISA::inc_sp(){write16($SP, inc_16(read16($SP)));}
/* ******************* DEC nn ******************* */
// Description:
//  Decrement register nn.
//
// Use with:
//  nn = BC,DE,HL,SP
//
// Flags affected:
//  None.
void ISA::dec_bc(){write16($BC, dec_16(read16($BC)));}
void ISA::dec_de(){write16($DE, dec_16(read16($DE)));}
void ISA::dec_hl(){write16($HL, dec_16(read16($HL)));}
void ISA::dec_sp(){write16($SP, dec_16(read16($SP)));}

/*************************** Rotate and shift *******************************/

/* ******************* RLC nn ******************* */
// Description:
//  Rotate A left. Old bit 7 to Carry flag.
//
// Flags affected:
// Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 7 data.
void ISA::rlca(){ write8($A, rotate_left_to_carry(read8($A)));}
/* ******************* RLA nn ******************* */
// Description:
//     Rotate A left through Carry flag.
// Flags affected:
//     Z - Set if result is zero.
//     N - Reset.
//     H - Reset.
//     C - Contains old bit 7 data.
void ISA::rla(){write8($A, rotate_left_through_carry(read8($A)));}
/* ******************* RRCA nn ******************* */
// Description:
//  Rotate A right. Old bit 0 to Carry flag.
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 0 data.
void ISA::rrca(){write8($A, rotate_right_to_carry(read8($A)));}
/* ******************* RRA nn ******************* */
// Description:
//     Rotate A right through Carry flag.
// Flags affected:
//      Z - Set if result is zero.
//      N - Reset.
//      H - Reset.
//      C - Contains old bit 0 data.
void ISA::rra(){write8($A, rotate_right_through_carry(read8($A)));}
/* ******************* RLC n ******************* */
// Description:
//  Rotate n left. Old bit 7 to Carry flag.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
// Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 7 data.
void ISA::rlc_a(){write8($A, rotate_left_to_carry(read8($A)));}
void ISA::rlc_b(){write8($B, rotate_left_to_carry(read8($B)));}
void ISA::rlc_c(){write8($C, rotate_left_to_carry(read8($C)));}
void ISA::rlc_d(){write8($D, rotate_left_to_carry(read8($D)));}
void ISA::rlc_e(){write8($E, rotate_left_to_carry(read8($E)));}
void ISA::rlc_h(){write8($H, rotate_left_to_carry(read8($H)));}
void ISA::rlc_l(){write8($L, rotate_left_to_carry(read8($L)));}
void ISA::rlc_hlp(){writeMem(read16($HL), rotate_left_to_carry(readMem(read16($HL))));}
/* ******************* RL n ******************* */
// Description:
//  Rotate n left through Carry flag.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 7 data.
void ISA::rl_a(){write8($A, rotate_left_through_carry(read8($A)));}
void ISA::rl_b(){write8($B, rotate_left_through_carry(read8($B)));}
void ISA::rl_c(){write8($C, rotate_left_through_carry(read8($C)));}
void ISA::rl_d(){write8($D, rotate_left_through_carry(read8($D)));}
void ISA::rl_e(){write8($E, rotate_left_through_carry(read8($E)));}
void ISA::rl_h(){write8($H, rotate_left_through_carry(read8($H)));}
void ISA::rl_l(){write8($L, rotate_left_through_carry(read8($L)));}
void ISA::rl_hlp(){writeMem(read16($HL), rotate_left_through_carry(readMem(read16($HL))));}
/* ******************* RRC n ******************* */
// Description:
//  Rotate n right. Old bit 0 to Carry flag.
// Use with:
// n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 0 data.
void ISA::rrc_a(){write8($A, rotate_right_to_carry(read8($A)));}
void ISA::rrc_b(){write8($B, rotate_right_to_carry(read8($B)));}
void ISA::rrc_c(){write8($C, rotate_right_to_carry(read8($C)));}
void ISA::rrc_d(){write8($D, rotate_right_to_carry(read8($D)));}
void ISA::rrc_e(){write8($E, rotate_right_to_carry(read8($E)));}
void ISA::rrc_h(){write8($H, rotate_right_to_carry(read8($H)));}
void ISA::rrc_l(){write8($L, rotate_right_to_carry(read8($L)));}
void ISA::rrc_hlp(){writeMem(read16($HL), rotate_right_to_carry(readMem(read16($HL))));}
/* ******************* RR n ******************* */
// Description:
//  Rotate n right through Carry flag.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 0 data.
void ISA::rr_a(){write8($A, rotate_right_through_carry(read8($A)));}
void ISA::rr_b(){write8($B, rotate_right_through_carry(read8($B)));}
void ISA::rr_c(){write8($C, rotate_right_through_carry(read8($C)));}
void ISA::rr_d(){write8($D, rotate_right_through_carry(read8($D)));}
void ISA::rr_e(){write8($E, rotate_right_through_carry(read8($E)));}
void ISA::rr_h(){write8($H, rotate_right_through_carry(read8($H)));}
void ISA::rr_l(){write8($L, rotate_right_through_carry(read8($L)));}
void ISA::rr_hlp(){writeMem(read16($HL), rotate_right_through_carry(readMem(read16($HL))));}
/* ******************* SLA n ******************* */
// Description:
// Shift n left into Carry. LSB of n set to 0. Use with:
//     n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//     Z - Set if result is zero.
//     N - Reset.
//     H - Reset.
//     C - Contains old bit 7 data.
void ISA::sla_a(){write8($A, shift_left_to_carry(read8($A)));}
void ISA::sla_b(){write8($B, shift_left_to_carry(read8($B)));}
void ISA::sla_c(){write8($C, shift_left_to_carry(read8($C)));}
void ISA::sla_d(){write8($D, shift_left_to_carry(read8($D)));}
void ISA::sla_e(){write8($E, shift_left_to_carry(read8($E)));}
void ISA::sla_h(){write8($H, shift_left_to_carry(read8($H)));}
void ISA::sla_l(){write8($L, shift_left_to_carry(read8($L)));}
void ISA::sla_hlp(){writeMem(read16($HL), shift_left_to_carry(readMem(read16($HL))));}
/* ******************* SRA n ******************* */
// Description:
//  Shift n right into Carry. MSB doesn't change.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 0 data.
void ISA::sra_a(){write8($A, arithmetic_shift_right_to_carry(read8($A)));}
void ISA::sra_b(){write8($B, arithmetic_shift_right_to_carry(read8($B)));}
void ISA::sra_c(){write8($C, arithmetic_shift_right_to_carry(read8($C)));}
void ISA::sra_d(){write8($D, arithmetic_shift_right_to_carry(read8($D)));}
void ISA::sra_e(){write8($E, arithmetic_shift_right_to_carry(read8($E)));}
void ISA::sra_h(){write8($H, arithmetic_shift_right_to_carry(read8($H)));}
void ISA::sra_l(){write8($L, arithmetic_shift_right_to_carry(read8($L)));}
void ISA::sra_hlp(){writeMem(read16($HL), arithmetic_shift_right_to_carry(readMem(read16($HL))));}
/* ******************* SRL n ******************* */
// Description:
//  Shift n right into Carry. MSB set to 0.
// Use with:
//  n = A,B,C,D,E,H,L,(HL)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Contains old bit 0 data.
void ISA::srl_a(){write8($A, logic_shift_right_to_carry(read8($A)));}
void ISA::srl_b(){write8($B, logic_shift_right_to_carry(read8($B)));}
void ISA::srl_c(){write8($C, logic_shift_right_to_carry(read8($C)));}
void ISA::srl_d(){write8($D, logic_shift_right_to_carry(read8($D)));}
void ISA::srl_e(){write8($E, logic_shift_right_to_carry(read8($E)));}
void ISA::srl_h(){write8($H, logic_shift_right_to_carry(read8($H)));}
void ISA::srl_l(){write8($L, logic_shift_right_to_carry(read8($L)));}
void ISA::srl_hlp(){writeMem(read16($HL), logic_shift_right_to_carry(readMem(read16($HL))));}

/*************************** Restarts *******************************/
// Description:
//  Push present address onto stack.
//  Jump to address $0000 + n.
// Use with:
//  n = $00,$08,$10,$18,$20,$28,$30,$38
void ISA::rst_0(){
  push(read16($PC));
  write16($PC, 0x0000);
}

void ISA::rst_08(){
  push(read16($PC));
  write16($PC, 0x0008);
}

void ISA::rst_10(){
  push(read16($PC));
  write16($PC, 0x0010);
}

void ISA::rst_18(){
  push(read16($PC));
  write16($PC, 0x0018);
}

void ISA::rst_20(){
  push(read16($PC));
  write16($PC, 0x0020);
}

void ISA::rst_28(){
  push(read16($PC));
  write16($PC, 0x0028);
}

void ISA::rst_30(){
  push(read16($PC));
  write16($PC, 0x0030);
}

void ISA::rst_38(){
   push(read16($PC));
   write16($PC, 0x0038);
}

/*************************** Returns *******************************/

/* ******************* RET n ******************* */
// Description:
//  Pop two bytes from stack & jump to that address.
void ISA::ret(){ write16($PC, pop());}
/* ******************* RET cc ******************* */
// Description:
//  Return if following condition is true:
// Use with:
//  cc = NZ, Return if Z flag is reset.
//  cc = Z, Return if Z flag is set.
//  cc = NC, Return if C flag is reset.
//  cc = C, Return if C flag is set.
void ISA::ret_nz(){
  if(!(Register::isZero())){
    ret();
    CPU::incrClockBy(20);
  } else CPU::incrClockBy(8);
}

void ISA::ret_z(){
  if(Register::isZero()){
    ret();
    CPU::incrClockBy(20);
  } else CPU::incrClockBy(8);
}

void ISA::ret_nc(){
  if(!Register::isCarry()){
    ret();
    CPU::incrClockBy(20);
  }else CPU::incrClockBy(8);
}

void ISA::ret_c(){
  if(Register::isCarry()){
    ret();
    CPU::incrClockBy(20);
  }else CPU::incrClockBy(8);
}
/* ******************* RETI cc ******************* */
// Description:
//  Pop two bytes from stack & jump to that address then
//  enable interrupts.
void ISA::reti(){
  //std::cout<<"enabled";
  write16($PC, pop());
  master_enable = 1;
}
/*************************** Bit Opcodes *******************************/

/* ******************* BIT b,r ******************* */
// Description:
// Test bit b in register r.
// Use with:
//   b = 0 - 7, r = A,B,C,D,E,H,L,(HL)
// Flags affected:
//   Z - Set if bit b of register r is 0.
//   N - Reset.
//   H - Set.
//   C - Not affected.
void ISA::bit_0_a(){bit(0,read8($A));}
void ISA::bit_0_b(){bit(0,read8($B));}
void ISA::bit_0_c(){bit(0,read8($C));}
void ISA::bit_0_d(){bit(0,read8($D));}
void ISA::bit_0_e(){bit(0,read8($E));}
void ISA::bit_0_h(){bit(0,read8($H));}
void ISA::bit_0_l(){bit(0,read8($L));}
void ISA::bit_0_hlp(){bit(0,readMem(read16($HL)));}

void ISA::bit_1_a(){bit(1,read8($A));}
void ISA::bit_1_b(){bit(1,read8($B));}
void ISA::bit_1_c(){bit(1,read8($C));}
void ISA::bit_1_d(){bit(1,read8($D));}
void ISA::bit_1_e(){bit(1,read8($E));}
void ISA::bit_1_h(){bit(1,read8($H));}
void ISA::bit_1_l(){bit(1,read8($L));}
void ISA::bit_1_hlp(){bit(1,readMem(read16($HL)));}

void ISA::bit_2_a(){bit(2,read8($A));}
void ISA::bit_2_b(){bit(2,read8($B));}
void ISA::bit_2_c(){bit(2,read8($C));}
void ISA::bit_2_d(){bit(2,read8($D));}
void ISA::bit_2_e(){bit(2,read8($E));}
void ISA::bit_2_h(){bit(2,read8($H));}
void ISA::bit_2_l(){bit(2,read8($L));}
void ISA::bit_2_hlp(){bit(2,readMem(read16($HL)));}

void ISA::bit_3_a(){bit(3,read8($A));}
void ISA::bit_3_b(){bit(3,read8($B));}
void ISA::bit_3_c(){bit(3,read8($C));}
void ISA::bit_3_d(){bit(3,read8($D));}
void ISA::bit_3_e(){bit(3,read8($E));}
void ISA::bit_3_h(){bit(3,read8($H));}
void ISA::bit_3_l(){bit(3,read8($L));}
void ISA::bit_3_hlp(){bit(3,readMem(read16($HL)));}

void ISA::bit_4_a(){bit(4,read8($A));}
void ISA::bit_4_b(){bit(4,read8($B));}
void ISA::bit_4_c(){bit(4,read8($C));}
void ISA::bit_4_d(){bit(4,read8($D));}
void ISA::bit_4_e(){bit(4,read8($E));}
void ISA::bit_4_h(){bit(4,read8($H));}
void ISA::bit_4_l(){bit(4,read8($L));}
void ISA::bit_4_hlp(){bit(4,readMem(read16($HL)));}

void ISA::bit_5_a(){bit(5,read8($A));}
void ISA::bit_5_b(){bit(5,read8($B));}
void ISA::bit_5_c(){bit(5,read8($C));}
void ISA::bit_5_d(){bit(5,read8($D));}
void ISA::bit_5_e(){bit(5,read8($E));}
void ISA::bit_5_h(){bit(5,read8($H));}
void ISA::bit_5_l(){bit(5,read8($L));}
void ISA::bit_5_hlp(){bit(5,readMem(read16($HL)));}

void ISA::bit_6_a(){bit(6,read8($A));}
void ISA::bit_6_b(){bit(6,read8($B));}
void ISA::bit_6_c(){bit(6,read8($C));}
void ISA::bit_6_d(){bit(6,read8($D));}
void ISA::bit_6_e(){bit(6,read8($E));}
void ISA::bit_6_h(){bit(6,read8($H));}
void ISA::bit_6_l(){bit(6,read8($L));}
void ISA::bit_6_hlp(){bit(6,readMem(read16($HL)));}

void ISA::bit_7_a(){bit(7,read8($A));}
void ISA::bit_7_b(){bit(7,read8($B));}
void ISA::bit_7_c(){bit(7,read8($C));}
void ISA::bit_7_d(){bit(7,read8($D));}
void ISA::bit_7_e(){bit(7,read8($E));}
void ISA::bit_7_h(){bit(7,read8($H));}
void ISA::bit_7_l(){bit(7,read8($L));}
void ISA::bit_7_hlp(){bit(7,readMem(read16($HL)));}


/* ******************* SET b,r ******************* */
// Description:
//  Set bit b in register r.
// Use with:
//  b = 0 - 7
//  r = A,B,C,D,E,H,L,(HL)
void ISA::set_0_a(){write8($A,set(0,read8($A)));}
void ISA::set_0_b(){write8($B,set(0,read8($B)));}
void ISA::set_0_c(){write8($C,set(0,read8($C)));}
void ISA::set_0_d(){write8($D,set(0,read8($D)));}
void ISA::set_0_e(){write8($E,set(0,read8($E)));}
void ISA::set_0_h(){write8($H,set(0,read8($H)));}
void ISA::set_0_l(){write8($L,set(0,read8($L)));}
void ISA::set_0_hlp(){writeMem(read16($HL),set(0,read16($HL)));}

void ISA::set_1_a(){write8($A,set(1,read8($A)));}
void ISA::set_1_b(){write8($B,set(1,read8($B)));}
void ISA::set_1_c(){write8($C,set(1,read8($C)));}
void ISA::set_1_d(){write8($D,set(1,read8($D)));}
void ISA::set_1_e(){write8($E,set(1,read8($E)));}
void ISA::set_1_h(){write8($H,set(1,read8($H)));}
void ISA::set_1_l(){write8($L,set(1,read8($L)));}
void ISA::set_1_hlp(){writeMem(read16($HL),set(1,read16($HL)));}

void ISA::set_2_a(){write8($A,set(2,read8($A)));}
void ISA::set_2_b(){write8($B,set(2,read8($B)));}
void ISA::set_2_c(){write8($C,set(2,read8($C)));}
void ISA::set_2_d(){write8($D,set(2,read8($D)));}
void ISA::set_2_e(){write8($E,set(2,read8($E)));}
void ISA::set_2_h(){write8($H,set(2,read8($H)));}
void ISA::set_2_l(){write8($L,set(2,read8($L)));}
void ISA::set_2_hlp(){writeMem(read16($HL),set(2,read16($HL)));}

void ISA::set_3_a(){write8($A,set(3,read8($A)));}
void ISA::set_3_b(){write8($B,set(3,read8($B)));}
void ISA::set_3_c(){write8($C,set(3,read8($C)));}
void ISA::set_3_d(){write8($D,set(3,read8($D)));}
void ISA::set_3_e(){write8($E,set(3,read8($E)));}
void ISA::set_3_h(){write8($H,set(3,read8($H)));}
void ISA::set_3_l(){write8($L,set(3,read8($L)));}
void ISA::set_3_hlp(){writeMem(read16($HL),set(3,read16($HL)));}

void ISA::set_4_a(){write8($A,set(4,read8($A)));}
void ISA::set_4_b(){write8($B,set(4,read8($B)));}
void ISA::set_4_c(){write8($C,set(4,read8($C)));}
void ISA::set_4_d(){write8($D,set(4,read8($D)));}
void ISA::set_4_e(){write8($E,set(4,read8($E)));}
void ISA::set_4_h(){write8($H,set(4,read8($H)));}
void ISA::set_4_l(){write8($L,set(4,read8($L)));}
void ISA::set_4_hlp(){writeMem(read16($HL),set(4,read16($HL)));}

void ISA::set_5_a(){write8($A,set(5,read8($A)));}
void ISA::set_5_b(){write8($B,set(5,read8($B)));}
void ISA::set_5_c(){write8($C,set(5,read8($C)));}
void ISA::set_5_d(){write8($D,set(5,read8($D)));}
void ISA::set_5_e(){write8($E,set(5,read8($E)));}
void ISA::set_5_h(){write8($H,set(5,read8($H)));}
void ISA::set_5_l(){write8($L,set(5,read8($L)));}
void ISA::set_5_hlp(){writeMem(read16($HL),set(5,read16($HL)));}

void ISA::set_6_a(){write8($A,set(6,read8($A)));}
void ISA::set_6_b(){write8($B,set(6,read8($B)));}
void ISA::set_6_c(){write8($C,set(6,read8($C)));}
void ISA::set_6_d(){write8($D,set(6,read8($D)));}
void ISA::set_6_e(){write8($E,set(6,read8($E)));}
void ISA::set_6_h(){write8($H,set(6,read8($H)));}
void ISA::set_6_l(){write8($L,set(6,read8($L)));}
void ISA::set_6_hlp(){writeMem(read16($HL),set(6,read16($HL)));}

void ISA::set_7_a(){write8($A,set(7,read8($A)));}
void ISA::set_7_b(){write8($B,set(7,read8($B)));}
void ISA::set_7_c(){write8($C,set(7,read8($C)));}
void ISA::set_7_d(){write8($D,set(7,read8($D)));}
void ISA::set_7_e(){write8($E,set(7,read8($E)));}
void ISA::set_7_h(){write8($H,set(7,read8($H)));}
void ISA::set_7_l(){write8($L,set(7,read8($L)));}
void ISA::set_7_hlp(){writeMem(read16($HL),set(7,read16($HL)));}


/* ******************* RES b,r ******************* */
// Description:
//  Reset bit b in register r.
// Use with:
//  b = 0 - 7
//  r = A,B,C,D,E,H,L,(HL)
void ISA::res_0_a(){write8($A,res(0,read8($A)));}
void ISA::res_0_b(){write8($B,res(0,read8($B)));}
void ISA::res_0_c(){write8($C,res(0,read8($C)));}
void ISA::res_0_d(){write8($D,res(0,read8($D)));}
void ISA::res_0_e(){write8($E,res(0,read8($E)));}
void ISA::res_0_h(){write8($H,res(0,read8($H)));}
void ISA::res_0_l(){write8($L,res(0,read8($L)));}
void ISA::res_0_hlp(){writeMem(read16($HL),res(0,read16($HL)));}

void ISA::res_1_a(){write8($A,res(1,read8($A)));}
void ISA::res_1_b(){write8($B,res(1,read8($B)));}
void ISA::res_1_c(){write8($C,res(1,read8($C)));}
void ISA::res_1_d(){write8($D,res(1,read8($D)));}
void ISA::res_1_e(){write8($E,res(1,read8($E)));}
void ISA::res_1_h(){write8($H,res(1,read8($H)));}
void ISA::res_1_l(){write8($L,res(1,read8($L)));}
void ISA::res_1_hlp(){writeMem(read16($HL),res(1,read16($HL)));}

void ISA::res_2_a(){write8($A,res(2,read8($A)));}
void ISA::res_2_b(){write8($B,res(2,read8($B)));}
void ISA::res_2_c(){write8($C,res(2,read8($C)));}
void ISA::res_2_d(){write8($D,res(2,read8($D)));}
void ISA::res_2_e(){write8($E,res(2,read8($E)));}
void ISA::res_2_h(){write8($H,res(2,read8($H)));}
void ISA::res_2_l(){write8($L,res(2,read8($L)));}
void ISA::res_2_hlp(){writeMem(read16($HL),res(2,read16($HL)));}

void ISA::res_3_a(){write8($A,res(3,read8($A)));}
void ISA::res_3_b(){write8($B,res(3,read8($B)));}
void ISA::res_3_c(){write8($C,res(3,read8($C)));}
void ISA::res_3_d(){write8($D,res(3,read8($D)));}
void ISA::res_3_e(){write8($E,res(3,read8($E)));}
void ISA::res_3_h(){write8($H,res(3,read8($H)));}
void ISA::res_3_l(){write8($L,res(3,read8($L)));}
void ISA::res_3_hlp(){writeMem(read16($HL),res(3,read16($HL)));}

void ISA::res_4_a(){write8($A,res(4,read8($A)));}
void ISA::res_4_b(){write8($B,res(4,read8($B)));}
void ISA::res_4_c(){write8($C,res(4,read8($C)));}
void ISA::res_4_d(){write8($D,res(4,read8($D)));}
void ISA::res_4_e(){write8($E,res(4,read8($E)));}
void ISA::res_4_h(){write8($H,res(4,read8($H)));}
void ISA::res_4_l(){write8($L,res(4,read8($L)));}
void ISA::res_4_hlp(){writeMem(read16($HL),res(4,read16($HL)));}

void ISA::res_5_a(){write8($A,res(5,read8($A)));}
void ISA::res_5_b(){write8($B,res(5,read8($B)));}
void ISA::res_5_c(){write8($C,res(5,read8($C)));}
void ISA::res_5_d(){write8($D,res(5,read8($D)));}
void ISA::res_5_e(){write8($E,res(5,read8($E)));}
void ISA::res_5_h(){write8($H,res(5,read8($H)));}
void ISA::res_5_l(){write8($L,res(5,read8($L)));}
void ISA::res_5_hlp(){writeMem(read16($HL),res(5,read16($HL)));}

void ISA::res_6_a(){write8($A,res(6,read8($A)));}
void ISA::res_6_b(){write8($B,res(6,read8($B)));}
void ISA::res_6_c(){write8($C,res(6,read8($C)));}
void ISA::res_6_d(){write8($D,res(6,read8($D)));}
void ISA::res_6_e(){write8($E,res(6,read8($E)));}
void ISA::res_6_h(){write8($H,res(6,read8($H)));}
void ISA::res_6_l(){write8($L,res(6,read8($L)));}
void ISA::res_6_hlp(){writeMem(read16($HL),res(6,read16($HL)));}

void ISA::res_7_a(){write8($A,res(7,read8($A)));}
void ISA::res_7_b(){write8($B,res(7,read8($B)));}
void ISA::res_7_c(){write8($C,res(7,read8($C)));}
void ISA::res_7_d(){write8($D,res(7,read8($D)));}
void ISA::res_7_e(){write8($E,res(7,read8($E)));}
void ISA::res_7_h(){write8($H,res(7,read8($H)));}
void ISA::res_7_l(){write8($L,res(7,read8($L)));}
void ISA::res_7_hlp(){writeMem(read16($HL),res(7,read16($HL)));}


/*************************** Calls *******************************/

/* ******************* CALL nn ******************* */
// Description:
//  Push address of next instruction onto stack and then
//  jump to address nn.
//
// Use with:
//  nn = two byte immediate value. (LS byte first.)
void ISA::call_nn(uint16_t operand){
  push(read16($PC));
  jp_nn(operand);
}


/* ******************* CALL n,n ******************* */
// Description:
//  Call address n if following condition is true:
//    cc = NZ, Call if Z flag is reset.
//    cc = Z, Call if Z flag is set.
//    cc = NC, Call if C flag is reset.
//    cc = C, Call if C flag is set.
// Use with:
//  nn = two byte immediate value. (LS byte first.)
void ISA::call_nz_nn(uint16_t operand){
  if(!Register::isZero()){
    call_nn(operand);
    CPU::incrClockBy(24);
  } else CPU::incrClockBy(12);
}

void ISA::call_z_nn(uint16_t operand){
   if(Register::isZero()){
     call_nn(operand);
     CPU::incrClockBy(24);
   } else CPU::incrClockBy(12);
 }

void ISA::call_nc_nn(uint16_t operand){
   if(!Register::isCarry()){
     call_nn(operand);
     CPU::incrClockBy(24);
   }else CPU::incrClockBy(12);
 }

void ISA::call_c_nn(uint16_t operand){
   if(Register::isCarry()){
     call_nn(operand);
     CPU::incrClockBy(24);
   }else CPU::incrClockBy(12);
 }

 /*************************** INTERRUPTS *******************************/
 void ISA::vblank_interrupt(){
  ISA::master_enable = 0;
  //std::cout << "V BLANK INTERRUPT" << std::endl;
  //SDL_RenderPresent( gRenderer );
  push(read16($PC));
  write16($PC, 0x40);
  CPU::incrClockBy(12);
}

void ISA::lcd_interrupt(){
  ISA::master_enable = 0;
  std::cout << "LCD INTERRUPT" << std::endl;
  push(read16($PC));
  write16($PC, 0x48);
  CPU::incrClockBy(12);
}

void ISA::timer_interrupt(){
  std::cout << "TIMER INTERRUPT" << std::endl;

  ISA::master_enable = 0;
  push(read16($PC));
  write16($PC, 0x50);
  CPU::incrClockBy(12);
}
void ISA::serial_interrupt(){
  std::cout << "SERIAL INTERRUPT" << std::endl;
  ISA::master_enable = 0;
  push(read16($PC));
  write16($PC, 0x58);
  CPU::incrClockBy(12);
}
void ISA::joypad_interrupt(){
  std::cout << "JOYPAD INTERRUPT" << std::endl;
  ISA::master_enable = 0;
  push(read16($PC));
  write16($PC, 0x60);
  CPU::incrClockBy(12);
}


 /*************************** PRIVATE *******************************/
uint16_t ISA::swap_bytes(uint16_t n) {
  uint8_t firstByte  = n & 0xFF;
  uint8_t secondByte = (n >> 8) | 0xFF;
  uint16_t result = (firstByte << 8 | secondByte);
  return result;
}

;

void ISA::handle_interrupts(){
    const uint8_t vblank_i = 0x01;
    const uint8_t lcd_i    = 0x01 << 1;
    const uint8_t timer_i  = 0x01 << 2;
    const uint8_t serial_i = 0x01 << 3;
    const uint8_t joypad_i = 0x01 << 4;

  uint8_t interrupt_enable = Memory::read(0xFFFF);
  uint8_t interrupt_flags  = Memory::read(0xFF0F);

  // if ((interrupt_enable & lcd_i)) {
  //     std::cout << "enabled lcd interrpt" << std::endl;
  // }
  //
  // if ((interrupt_flags & lcd_i)) {
  //     std::cout << "lcd interrpt fired" << std::endl;
  // }
  //

  //check if interrupt is needed
  if(ISA::master_enable && interrupt_enable && interrupt_flags){
    //std::cout << "ENTERING HANDLING INTERRUPTS" << std::endl;
    uint8_t fired = interrupt_enable & interrupt_flags;

    if(fired & vblank_i){ //check vblank
      interrupt_flags &= (0xFF - vblank_i);
      ISA::vblank_interrupt();
    }


    if(fired & lcd_i){ //check lcd
      interrupt_flags &= (0xFF - lcd_i); //zero out the flag of the interupt
      ISA::lcd_interrupt();
    }

    if(fired & timer_i){ //check time
      interrupt_flags &= (0xFF - timer_i);
      ISA::timer_interrupt();
    }

    if(fired & serial_i){ //check joypad
      interrupt_flags &= (0xFF - serial_i);
      ISA::serial_interrupt();
    }

    if(fired & joypad_i){ //check joypad
      interrupt_flags &= (0xFF - joypad_i);
      ISA::joypad_interrupt();
    }

    Memory::write(0xFF0F,interrupt_flags);
  }
}
