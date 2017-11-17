#include "../include/alu.hpp"

ALU::ALU(){};
ALU::~ALU(){};


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
Description:
 Add n to A.
Flags affected:
 Z - Set if result is zero.
 N - Reset.
 H - Set if carry from bit 3.
 C - Set if carry from bit 7.
*/

//NOTE:changed get rid of the extra argument, since add is only used for a
uint8_t ALU::add_8(uint8_t b) {
    unsigned int result     = Register::read8($A) + b;
    unsigned int halfResult = (Register::read8($A) & 0x0F) + (b & 0x0F);
    uint8_t returnResult    = (uint8_t) (result & 0xFF);
    //C flag
    if (result > 0xFF) Register::set_flag_carry();
    else Register::reset_flag_carry();

    //H flag
    if (halfResult > 0x0F) Register::set_flag_half_carry();
    else Register::reset_flag_half_carry();

    //Z flag
    if (returnResult == 0  ) Register::set_flag_zero();
    else Register::reset_flag_zero();

    //N flag
    Register::reset_flag_sub();

    return returnResult;
}


/*
Description:
 Add n + Carry flag to A.
Use with:

 Z - Set if result is zero.
 N - Reset.
 H - Set if carry from bit 3.
 C - Set if carry from bit 7.
*/
//NOTE: got rid of extra argument since it is always performed on A
uint8_t ALU::adc_8(uint8_t b){
  uint8_t carry           = (((uint8_t) Register::isCarry()) & 1 );
  unsigned int result     = Register::read8($A) + b + carry;
  unsigned int halfResult = (Register::read8($A) & 0x0F) + (b & 0x0F) + carry;
  uint8_t returnResult    = (uint8_t) (result & 0xFF);


  //C flag
  if (result > 0xFF) Register::set_flag_carry();
  else Register::reset_flag_carry();

  //H flag
  if (halfResult > 0x0F) Register::set_flag_half_carry();
  else Register::reset_flag_half_carry();

  //Zero flag
  if (returnResult == 0) Register::set_flag_zero();
  else Register::reset_flag_zero();

  //N flag
  Register::reset_flag_sub();

  return returnResult;
}


/*
Description:
 Subtract n from A.

Flags affected:
 Z - Set if result is zero.
 N - Set.
 H - Set if no borrow from bit 4.
 C - Set if no borrow.
*/
uint8_t ALU::sub_8(uint8_t b){
  //C flag
  if (Register::read8($A) < b) Register::set_flag_carry();
  else Register::reset_flag_carry();

  //H flag
  if ((Register::read8($A) & 0x0F) < (b & 0x0F)) Register::set_flag_half_carry();
  else Register::reset_flag_half_carry();

  //Z flag
  if (Register::read8($A) == b) Register::set_flag_zero();
  else Register::reset_flag_zero();

  Register::set_flag_sub();
  return Register::read8($A) - b;
}


/*
Description:
 Subtract n + Carry flag from A.

Flags affected:
 Z - Set if result is zero.
 N - Set.
 H - Set if no borrow from bit 4.
 C - Set if no borrow.
*/

uint8_t ALU::sbc_8(uint8_t b){
  //C flag

  uint8_t carry = Register::isCarry();

  if (Register::read8($A) < (b + carry)) Register::set_flag_carry();
  else Register::reset_flag_carry();

  //H flag
  if ((Register::read8($A) & 0x0F) < ((b & 0x0F) + carry)) Register::set_flag_half_carry();
  else Register::reset_flag_half_carry();

  //Z flag
  if (Register::read8($A) == (b + carry)) Register::set_flag_zero();
  else Register::reset_flag_zero();


  Register::set_flag_sub();
  return Register::read8($A) - b - carry;
}



/*
Description:
 Logically AND n with A, result in A.

Flags affected:
 Z - Set if result is zero.
 N - Reset.
 H - Set.
 C - Reset.

NOTE: this instruction DOES NOT have a destination specified because it is implied as ALWAYS being register A.
*/
uint8_t ALU::and_8(uint8_t b) {
  uint8_t isTrue = (Register::read8($A) & b);

  if (!isTrue) Register::set_flag_zero();
  else Register::reset_flag_zero();
  // Carry
  Register::reset_flag_carry();

  // half carry
  Register::set_flag_half_carry();

  // sub
  Register::reset_flag_sub();
  return isTrue;
}


/*
Description:
 Logical OR n with register A, result in A.

Flags affected:
 Z - Set if result is zero.
 N - Reset.
 H - Reset.
 C - Reset.
*/
uint8_t ALU::or_8(uint8_t b) {

  uint8_t isFalse = (Register::read8($A) | b);

  if (!isFalse) Register::set_flag_zero();
  else Register::reset_flag_zero();

  Register::reset_flag_half_carry();

  Register::reset_flag_carry();

  Register::reset_flag_sub();

  return isFalse;
}


/*
Description:
 Logical exclusive OR n with register A, result in A.

Flags affected:
 Z - Set if result is zero.
 N - Reset.
 H - Reset.
 C - Reset.
*/
uint8_t ALU::xor_8(uint8_t b) {
  uint8_t isFalse = (Register::read8($A) ^ b);

  if (!isFalse) Register::set_flag_zero();
  else Register::reset_flag_zero();

  Register::reset_flag_half_carry();

  Register::reset_flag_carry();

  Register::reset_flag_sub();

  return isFalse;
}


/*
Description:
 Compare A with n. This is basically an A - n
 subtraction instruction but the results are thrown
 away.

Flags affected:
 Z - Set if result is zero. (Set if A = n.)
 N - Set.
 H - Set if no borrow from bit 4.
 C - Set for no borrow. (Set if A < n.)
*/
void ALU::cp_8(uint8_t b) {
  //C flag
  if (Register::read8($A) < b) Register::set_flag_carry();
  else Register::reset_flag_carry();

  //H flag
  if ((Register::read8($A) & 0x0F) < (b & 0x0F)) Register::set_flag_half_carry();
  else Register::reset_flag_half_carry();

  //Z flag
  if (Register::read8($A) == b) Register::set_flag_zero();
  else Register::reset_flag_zero();

  Register::set_flag_sub();
}



/*
Description:
 Increment register n.

Flags affected:
 Z - Set if result is zero.
 N - Reset.
 H - Set if carry from bit 3.
 C - Not affected.
*/
uint8_t ALU::inc_8(uint8_t value) {
  unsigned int result     = value + 1;
  unsigned int halfResult = (value & 0x0F) + 1;
  uint8_t returnResult    = (uint8_t)(result & 0xFF);

  Register::reset_flag_sub();

  if (halfResult > 0x0F) Register::set_flag_half_carry();
  else Register::reset_flag_half_carry();

  if (returnResult == 0 ) Register::set_flag_zero();
  else Register::reset_flag_zero();

  return returnResult;
}

/*
Description:
 Decrement register n.

Flags affected:
 Z - Set if reselt is zero.
 N - Set.
 H - Set if no borrow from bit 4.
 C - Not affected.
*/

uint8_t ALU::dec_8(uint8_t value) {
  unsigned int result     = value - 1;
  unsigned int halfResult = (value & 0x0F) - 1;
  uint8_t returnResult    = (uint8_t)(result & 0xFF);

  Register::set_flag_sub();

  if (halfResult > 0x0F) Register::set_flag_half_carry();
  else Register::reset_flag_half_carry();

  if (returnResult == 0  ) Register::set_flag_zero();
  else Register::reset_flag_zero();

  return returnResult;
}


uint16_t ALU::inc_16(uint16_t value) { return value + 0x01; }


uint16_t ALU::dec_16(uint16_t value) { return value - 0x01; }



/*
Description:
 Add n to HL.

Flags affected:
 Z - Not affected.
 N - Reset.
 H - Set if carry from bit 11.
 C - Set if carry from bit 15.
*/
uint16_t ALU::add_hl(uint16_t b) {
    unsigned int result     = Register::read16($HL) + b;
    unsigned int halfResult = (Register::read16($HL) & 0x0FFF) + (b & 0x0FFF);
    uint16_t returnResult   = (uint16_t) (result & 0xFFFF);
    Register::reset_flag_sub();

    if (result     > 0xFFFF) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if (halfResult > 0x1000) Register::set_flag_half_carry();
    else Register::reset_flag_half_carry();

    return returnResult;
}

/*
Description:
 Add n to Stack Pointer (SP).

Flags affected:
 Z - Reset.
 N - Reset.
 H - Set or reset according to operation.
 C - Set or reset according to operation.
*/
uint16_t ALU::add_sp(int8_t b) {
    int result = Register::read16($SP) + b;

    if(result & 0xFFFF0000) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if(((Register::read16($SP) & 0x0F) + b) > 0x0F) Register::set_flag_half_carry();
    else Register::reset_flag_half_carry();

    Register::reset_flag_sub();
    Register::reset_flag_zero();

    return (uint16_t) result & 0xFFFF;
}


/*
Rotate Left Circular Accumulator. This instruction rotates A left
one bit, placing bit 7 at bit 0 AND in the Carry flag.
Flags affected:
Z - Set if result is zero.
N - Reset.
H - Reset.
C - Contains old bit 7 data.
*/
uint8_t ALU::rotate_left_to_carry(uint8_t a){
   uint8_t bit7      = a & 0x80;
   uint8_t newBit0   = bit7 >> 7;
   uint8_t result    = a << 1;

   result = result | newBit0;

   if (newBit0) Register::set_flag_carry();
   else Register::reset_flag_carry();
   if (!result) Register::set_flag_zero();
   else Register::reset_flag_zero();

   Register::reset_flag_sub();
   Register::reset_flag_half_carry();


   return result;
}



/*
This instruction rotates A left one bit, placing bit 7
into the Carry flag and the contents of the Carry flag
into bit 0 of A
Z - Set if result is zero.
N - Reset.
H - Reset.
C - Contains old bit 7 data
*/
uint8_t ALU::rotate_left_through_carry(uint8_t a){
    uint8_t carry     = (uint8_t) Register::isCarry();
    uint8_t bit7      = a  & 0x80;
    uint8_t result    = a << 1;

    result = result | carry;

    if (bit7) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if (!result) Register::set_flag_zero();
    else Register::reset_flag_zero();

    Register::reset_flag_sub();
    Register::reset_flag_half_carry();

    return result;
}


/*
Rotate A right. Old bit 0 to Carry flag.
Flags affected:
Z - Set if result is zero.
N - Reset.
H - Reset.
C - Contains old bit 0 data.
*/
uint8_t ALU::rotate_right_to_carry(uint8_t a) {
    uint8_t bit0   = a & 0x01;
    uint8_t bit7   = bit0 << 7;
    uint8_t result = a >> 1;

    result = result | bit7;
    if (bit0) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if (!result) Register::set_flag_zero();
    else Register::reset_flag_zero();

    Register::reset_flag_sub();
    Register::reset_flag_half_carry();

    return result;
}



/*
This instruction rotates A right one bit,
placing bit 0 into the Carry flag and the
contents of the Carry flag into bit 7 of A

Z - Set if result is zero.
N - Reset.
H - Reset.
C - Contains old bit 0 data.
*/
uint8_t ALU::rotate_right_through_carry(uint8_t a){
    uint8_t carry     = (uint8_t) Register::isCarry();
    uint8_t bit0      = a & 0x01;
    uint8_t result    = a >> 1;

    result = result | (carry << 7);
    if (bit0) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if (!result) Register::set_flag_zero();
    else Register::reset_flag_zero();

    Register::reset_flag_sub();
    Register::reset_flag_half_carry();

    return result;
}


/*
Shift n left into Carry. Least Significant Bit, bit0, of n set to 0.
Z - Set if result is zero.
N - Reset.
H - Reset.
C - Contains old bit 0 data.
*/
uint8_t ALU::shift_left_to_carry(uint8_t a){
    uint8_t bit7  = a & 0x80;
    uint8_t result   = a << 1;

    if (bit7) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if (!result) Register::set_flag_zero();
    else Register::reset_flag_zero();

    Register::reset_flag_half_carry();
    Register::reset_flag_sub();

    return result;
}

 /*
 Shift n right into Carry. MSB doesn't change.
 https://en.wikipedia.org/wiki/Arithmetic_shift
 Z - Set if result is zero.
 N - Reset.
 H - Reset.
 C - Contains old bit 0 data.
 */
uint8_t ALU::arithmetic_shift_right_to_carry(uint8_t a){
    uint8_t bit7   = a & 0x80;
    uint8_t bit0   = a & 0x01;
    uint8_t result = (a >> 1) | bit7;

    if (bit0) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if (!result) Register::set_flag_zero();
    else Register::reset_flag_zero();

    Register::reset_flag_half_carry();
    Register::reset_flag_sub();

    return result;
}

 /*
 Shift n right into Carry. MSB set to 0.
 Flags affected:
 Z - Set if result is zero.
 N - Reset.
 H - Reset.
 C - Contains old bit 0 data.
 */
uint8_t ALU::logic_shift_right_to_carry(uint8_t a){
    uint8_t bit0   = a & 0x01;
    uint8_t result = a >> 1;

    if (bit0) Register::set_flag_carry();
    else Register::reset_flag_carry();

    if (!result) Register::set_flag_zero();
    else Register::reset_flag_zero();

    Register::reset_flag_half_carry();
    Register::reset_flag_sub();
    return result;
}

/*
Tests bit b in register r or the byte addressed in HL. Basically the specified
bit gets copied to the Z flag AND INVERTED. Why inverted? Because the Z flag is
set when a result is ZERO, that's why. =)
Z - Set if bit b of register r is 0.
N - Reset.
H - Set.
C - Not affected.
*/

void ALU::bit(uint8_t bit_no, uint8_t b){
  uint8_t result = b & (0x01 << bit_no);

    if (!result) Register::set_flag_zero();
    else Register::reset_flag_zero();

    Register::set_flag_half_carry();
    Register::reset_flag_sub();
}

/*
Set bit b in register r.
No flags are affected.
*/
uint8_t ALU::set(uint8_t bit_no, uint8_t b){
    uint8_t result = b | (0x01 << bit_no);
    return result;
}


/*
Reset bit b in register r.
No flags are affected.
*/
uint8_t ALU::reset(uint8_t bit_no, uint8_t b){
    uint8_t resetBit = 0x01 << bit_no;
    uint8_t result = (b | resetBit) ^ resetBit;
    return result;
}

/*
Description:
  Swap upper & lower nibles of n.

Flags affected:
  Z - Set if result is zero.
  N - Reset.
  H - Reset.
  C - Reset.
*/
uint8_t ALU::swap(uint8_t value) {
  uint8_t result = ((value & 0xf) << 4) | ((value & 0xf0) >> 4);

	if(!value) Register::set_flag_zero();
	else Register::reset_flag_zero();

  Register::reset_flag_sub();
  Register::reset_flag_carry();
  Register::reset_flag_half_carry();

	return result;
}
