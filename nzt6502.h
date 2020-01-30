#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class Bus;

class nzt6502
{
public:
  nzt6502();
  ~nzt6502();

  void ConnectBus(Bus *n) { bus = n; }

  // addressing modes
  // effectively, where is the data for the opcode?
  // http://www.emulator101.com/6502-addressing-modes.html
  // basically, we get an instruction, modify it based on the addressing mode, then execute
  // https://github.com/OneLoneCoder/olcNES/blob/master/Part%232%20-%20CPU/olc6502.h#L204-L218
  uint8_t IMP();
  uint8_t IMM();
  uint8_t ZP0();
  uint8_t ZPX();
  uint8_t ZPY();
  uint8_t REL();
  uint8_t ABS();
  uint8_t ABX();
  uint8_t ABY();
  uint8_t IND();
  uint8_t IZX();
  uint8_t IZY();

  // Opcodes IM COPYING AND PASTING
  // 56 total opcodes supported by the NES
  // each is one byte
  // as a result, there are a potential 256 opcodes (16 x 16)
  // some operations are tagged with extra data

  // each opcode function should return 0 UNLESS
  //they require extra clock cycles when combined with certain addressing modes
  // then they return 1

  uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

	// functionally identical to a NOP
	uint8_t XXX();

  // force a clock cycle
  void clock();
  void reset(); // reset interrupt - forces the CPU into a known state
  void irq(); // Interrupt request
  void nmi(); // "non-maskable interrupt" - same as irq() but cannot be disabled

  uint8_t fetch();
  uint16_t fetched = 0x00;

  // depending on addressing mode, we read from different areas of memory
  uint16_t addr_abs = 0x0000;
  uint16_t addr_res = 0x0000;
  uint8_t opcode = 0x00;
  uint8_t cycles = 0; // how many cycles are left for the instruction

public:

  // The Status register has 8 bits. Each bit refers to a flag. By left-shifting, we can access the flag directly
  enum FLAGS6502
  {
    C = (1 << 0), // Carry Bit
    Z = (1 << 1), // Zero
    I = (1 << 2), // Disable interrupts
    D = (1 << 3), // Decimal Mode
    B = (1 << 4), // Break
    U = (1 << 5), // Unused
    V = (1 << 6), // Overflow
    N = (1 << 7), // Negative
  };

  uint8_t a = 0x00; // accumulator register
  uint8_t x = 0x00; // X register
  uint8_t y = 0x00; // Y register
  uint8_t sp = 0x00; // Stack pointer  (where on the bus are we), just like VM
  uint16_t pc = 0x0000; // Program counter
  uint8_t status = 0x00; // status register


private:
  Bus *bus = nullptr;
  void write(uint16_t addr, uint8_t data);
  uint8_t read(uint16_t addr, bool bReadOnly = false);

  // methods to access status register
  uint8_t GetFlag(FLAGS6502 f);
  void SetFlag(FLAGS6502 f, bool v);

  // need to add the 16 x 16 table of Opcodes

  struct INSTRUCTION {
    std::string name;
    uint8_t(nzt6502::*operate)(void) = nullptr;
    uint8_t(nzt6502::*addrmode)(void) = nullptr;
    uint8_t cycles = 0;
  };

  std::vector<INSTRUCTION> lookup;
};