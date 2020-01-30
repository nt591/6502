#pragma once
#include <cstdint>
#include <array>

#include "nzt6502.h"

class Bus
{
public:
  Bus();
  ~Bus();

public: //devices on the bus
  nzt6502 cpu;
  // 64 kb of ram
  std::array<u_int8_t, 64 * 1024> ram;

public: // reading and writing to the bus
  void write(uint16_t addr, uint8_t data);
  uint8_t read(uint16_t addr, bool bReadOnly = false);
};
