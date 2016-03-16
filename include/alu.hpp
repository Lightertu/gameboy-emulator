#ifndef ALU_H_
#define ALU_H_

/*
    ALU does not write to any thing, thoes are pure functions
    However, it does change the flag setting.

*/

#include <iostream>
#include <sstream>
#include <bitset>
#include <cstdint>

#include "../include/register.hpp"

class ALU{
public:
  ALU();
  virtual ~ALU();
  //static uint8_t add_8(uint8_t a, uint8_t b);
  static uint8_t add_8(uint8_t b);

  // static uint8_t adc_8(uint8_t a, uint8_t b);
  static uint8_t adc_8( uint8_t b);
  static uint8_t sub_8(uint8_t a);

  static uint8_t sbc_8(uint8_t b);
  static uint8_t and_8(uint8_t a);
  static uint8_t  or_8(uint8_t a);
  static uint8_t xor_8(uint8_t a);
  static void    cp_8 (uint8_t a);
  static uint8_t dec_8(uint8_t a);
  static uint8_t inc_8(uint8_t a);


  /* 16 bits ALU operations */
  static uint16_t add_hl(uint16_t b);
  static uint16_t add_sp(int8_t  b);
  static uint16_t inc_16(uint16_t value);
  static uint16_t dec_16(uint16_t value);


  /* rotate and shift */
  static uint8_t rotate_left_to_carry(uint8_t a);
  static uint8_t rotate_left_through_carry(uint8_t a);
  static uint8_t rotate_right_to_carry(uint8_t a);
  static uint8_t rotate_right_through_carry(uint8_t a);
  static uint8_t shift_left_to_carry(uint8_t a);
  static uint8_t arithmetic_shift_right_to_carry(uint8_t a);
  static uint8_t logic_shift_right_to_carry(uint8_t a);

  /* bit opcode */
  static void bit(uint8_t bit_no, uint8_t b);
  static uint8_t set(uint8_t bit_no, uint8_t b);
  static uint8_t reset(uint8_t bit_no, uint8_t b);


  /* weird ones */
  static uint8_t swap(uint8_t value);
private:

};
#endif
