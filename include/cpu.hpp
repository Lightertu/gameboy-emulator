#ifndef CPU_H_
#define CPU_H_
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include "isa.hpp"
#include "register.hpp"
#include "memory.hpp"
#include "databank.hpp"
#include "gpu.hpp"


class CPU {
public:

  CPU();
  virtual~CPU();
  //extern struct Keys;
  //static void initialize_keys(struct Keys *keys);
  //static void reset_keys(struct Keys *keys);
    static void incrCPUTickBy(uint8_t incr);
    static void printInstruction(int hexValue);
    static void decode();
    static void extendedDecode();
    static void incrClockBy(uint8_t);
    static long getClock();
    static uint16_t getLastTick();
    static void incrPCby(uint8_t);
    static void run();
    static bool isBootDone();
    static void bootstrap();
    static void updateTileCollection(uint16_t address);
    static uint8_t getTilePixel(uint16_t tileIndex, uint16_t x, uint16_t y);



private:
    static long _clock;
    static uint16_t _lastTick;
  };





  #endif
