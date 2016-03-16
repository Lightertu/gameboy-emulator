#include <iostream>
#include "../include/key_presses.hpp"


KEYS::KEYS(){};
KEYS::~KEYS(){};

uint8_t KEYS::key_up;
uint8_t KEYS::key_down;
uint8_t KEYS::key_left;
uint8_t KEYS::key_right;
uint8_t KEYS::key_start;
uint8_t KEYS::key_select;
uint8_t KEYS::key_a;
uint8_t KEYS::key_b;
uint8_t KEYS::keys[2];
uint8_t KEYS::column;

void KEYS::initialize_keys(){
  KEYS::key_up = 0;
  KEYS::key_down = 0;
  KEYS::key_left= 0;
  KEYS::key_right = 0;

  KEYS::key_start = 0;
  KEYS::key_select = 0;
  KEYS::key_a = 0;
  KEYS::key_b = 0;

  KEYS::keys[0] = 0x0F;
  KEYS::keys[1] = 0x0F;

  KEYS::column = 0;
}

void KEYS::reset_keys(){
  KEYS::key_up = 0;
  KEYS::key_down = 0;
  KEYS::key_left = 0;
  KEYS::key_right = 0;

  KEYS::key_start = 0;
  KEYS::key_select = 0;
  KEYS::key_a = 0;
  KEYS::key_b = 0;
  

  KEYS::keys[0] = 0x0F;
  KEYS::keys[1] = 0x0F;
}

void KEYS::write_keys(uint8_t value){
  // Reset set 4th and 5th bit. This register is only written to in order
  // to set these bits
  KEYS::column = value & 0x30;
  //std::cout << (int)column<<std::endl;
}

u_int8_t KEYS::read_keys(){
  u_int8_t sum = 0;
  //std::cout<<"reading";
 if(KEYS::column == 0x10){
   if(KEYS::key_start)
    sum |= (1 << 3);
   if(KEYS::key_select)
    sum |= (1 << 2);
   if(KEYS::key_b)
    sum |= (1 << 1);
   if(KEYS::key_a)
    sum |= (1 << 0);
   //std::cout << "returning: " << (int) (0xC0 | 0x10 |((~0xF0) & (~sum))) << std::endl;
  return 0xC0 | 0x10 |((~0xF0) & (~sum));
 }else if(KEYS::column == 0x20){
   if(KEYS::key_down)
    sum |= (1 << 3);
   if(KEYS::key_up)
    sum |= (1 << 2);
   if(KEYS::key_left)
    sum |= (1 << 1);
   if(KEYS::key_right)
    sum |= (1 << 0);
   //std::cout << "returning: "  << (int) (0xC0 | 0x12 |((~0xF0) & (~sum))) << std::endl; 
  return 0xC0 | 0x20 |((~0xF0) & (~sum));
 }else{
  return 0xFF;
 }
}
