#pragma once

#include <vector>

#include "mos6502.h"
#include "emulation/memory_access.hpp"
#include "emulation/mos6502/opcode_metadata.hpp"

#define NEGATIVE  0x80
#define OVERFLOW  0x40
#define CONSTANT  0x20
#define BREAK     0x10
#define DECIMAL   0x08
#define INTERRUPT 0x04
#define ZERO      0x02
#define CARRY     0x01

using namespace std;

class exec_mos6502 : public mos6502
{
private:
     using operation = mos6502_metadata::operation;

     i_memory_access *memory_access;
     bool is_addr_imm = false;
     bool illegal_instruction = false;
     bool reset_access = false;
     bool instruction_running = false;
     bool opcode_fetch_seen = false;
     size_t instruction_reads = 0;
     mos6502_metadata::instruction active_instruction{
         operation::invalid, mos6502_metadata::addressing_mode::IMP};

     static thread_local exec_mos6502 *active_cpu;
     static uint8_t readBus(uint16_t address);
     static void writeBus(uint16_t address, uint8_t value);
     uint8_t readBusValue(uint16_t address);
     void writeBusValue(uint16_t address, uint8_t value);
     mos6502_metadata::instruction instructionAtPC() const;
     bool contains(const vector<operation> &instructions) const;

     vector<operation> callInstr{
         operation::BRK,
         operation::JSR};

     vector<operation> returnInstr{
         operation::RTI,
         operation::RTS};

     vector<operation> readInstrA{
         operation::ADC,
         operation::AND,
         operation::ASL_ACC,
         operation::BIT,
         operation::CMP,
         operation::EOR,
         operation::LSR_ACC,
         operation::ORA,
         operation::PHA,
         operation::PLA,
         operation::ROL_ACC,
         operation::ROR_ACC,
         operation::SBC,
         operation::STA,
         operation::TAX,
         operation::TAY};

     vector<operation> writeInstrA{
         operation::ADC,
         operation::AND,
         operation::ASL_ACC,
         operation::EOR,
         operation::LDA,
         operation::LSR_ACC,
         operation::ORA,
         operation::ROL_ACC,
         operation::ROR_ACC,
         operation::SBC,
         operation::TXA,
         operation::TYA};

     vector<operation> readInstrX{
         operation::CPX,
         operation::DEX,
         operation::INX,
         operation::STX,
         operation::TXA,
         operation::TXS};

     vector<operation> writeInstrX{
         operation::DEX,
         operation::INX,
         operation::LDX,
         operation::TAX,
         operation::TSX};

     vector<operation> readInstrY{
         operation::CPY,
         operation::DEY,
         operation::INY,
         operation::STY,
         operation::TYA};

     vector<operation> writeInstrY{
         operation::DEY,
         operation::INY,
         operation::LDY,
         operation::TAY};

     vector<operation> readInstrNegative{
         operation::ADC,
         operation::AND,
         operation::ASL,
         operation::ASL_ACC,
         operation::BIT,
         operation::BMI,
         operation::BPL,
         operation::CMP,
         operation::CPX,
         operation::CPY,
         operation::DEC,
         operation::DEX,
         operation::DEY,
         operation::EOR,
         operation::INC,
         operation::INX,
         operation::INY,
         operation::LDA,
         operation::LDX,
         operation::LDY,
         operation::LSR,
         operation::LSR_ACC,
         operation::ORA,
         operation::PLA,
         operation::ROL,
         operation::ROL_ACC,
         operation::ROR,
         operation::ROR_ACC,
         operation::SBC,
         operation::TAX,
         operation::TAY,
         operation::TSX,
         operation::TXA,
         operation::TYA};

     vector<operation> readInstrOverflow{
         operation::ADC,
         operation::BVC,
         operation::BVS,
         operation::CLV,
         operation::SBC};

     vector<operation> readInstrConstant{};

     vector<operation> readInstrBreak{};

     vector<operation> readInstrDecimal{
         operation::ADC,
         operation::CLD,
         operation::SBC,
         operation::SED};

     vector<operation> readInstrInterrupt{
         operation::BRK,
         operation::CLI,
         operation::SEI};

     vector<operation> readInstrZero{
         operation::ADC,
         operation::AND,
         operation::ASL,
         operation::ASL_ACC,
         operation::BEQ,
         operation::BIT,
         operation::BNE,
         operation::CMP,
         operation::CPX,
         operation::CPY,
         operation::DEC,
         operation::DEX,
         operation::DEY,
         operation::EOR,
         operation::INC,
         operation::INX,
         operation::INY,
         operation::LDA,
         operation::LDX,
         operation::LDY,
         operation::LSR,
         operation::LSR_ACC,
         operation::ORA,
         operation::PHA,
         operation::PLA,
         operation::ROL,
         operation::ROL_ACC,
         operation::ROR,
         operation::ROR_ACC,
         operation::SBC,
         operation::TAX,
         operation::TAY,
         operation::TSX,
         operation::TXA,
         operation::TYA};

     vector<operation> readInstrCarry{
         operation::ADC,
         operation::AND,
         operation::ASL,
         operation::ASL_ACC,
         operation::BCC,
         operation::BCS,
         operation::CLC,
         operation::CMP,
         operation::CPX,
         operation::CPY,
         operation::LSR,
         operation::LSR_ACC,
         operation::ROL,
         operation::ROL_ACC,
         operation::ROR,
         operation::ROR_ACC,
         operation::SBC,
         operation::SEC};

     vector<operation> writeInstrNegative{
         operation::ADC,
         operation::AND,
         operation::ASL,
         operation::ASL_ACC,
         operation::BIT,
         operation::CMP,
         operation::CPX,
         operation::CPY,
         operation::DEC,
         operation::DEX,
         operation::DEY,
         operation::EOR,
         operation::INC,
         operation::INX,
         operation::INY,
         operation::LDA,
         operation::LDX,
         operation::LDY,
         operation::LSR,
         operation::LSR_ACC,
         operation::ORA,
         operation::PLA,
         operation::ROL,
         operation::ROL_ACC,
         operation::ROR,
         operation::ROR_ACC,
         operation::SBC,
         operation::TAX,
         operation::TAY,
         operation::TSX,
         operation::TXA,
         operation::TYA};

     vector<operation> writeInstrOverflow{
         operation::ADC,
         operation::CLV,
         operation::SBC};

     vector<operation> writeInstrConstant{};

     vector<operation> writeInstrBreak{};

     vector<operation> writeInstrDecimal{
         operation::CLD,
         operation::SED};

     vector<operation> writeInstrInterrupt{
         operation::BRK,
         operation::CLI,
         operation::SEI};

     vector<operation> writeInstrZero{
         operation::ADC,
         operation::AND,
         operation::ASL,
         operation::ASL_ACC,
         operation::BIT,
         operation::CMP,
         operation::CPX,
         operation::CPY,
         operation::DEC,
         operation::DEX,
         operation::DEY,
         operation::EOR,
         operation::INC,
         operation::INX,
         operation::INY,
         operation::LDA,
         operation::LDX,
         operation::LDY,
         operation::LSR,
         operation::LSR_ACC,
         operation::ORA,
         operation::PHA,
         operation::PLA,
         operation::ROL,
         operation::ROL_ACC,
         operation::ROR,
         operation::ROR_ACC,
         operation::SBC,
         operation::TAX,
         operation::TAY,
         operation::TSX,
         operation::TXA,
         operation::TYA};

     vector<operation> writeInstrCarry{
         operation::ADC,
         operation::AND,
         operation::ASL,
         operation::ASL_ACC,
         operation::CLC,
         operation::CMP,
         operation::CPX,
         operation::CPY,
         operation::LSR,
         operation::LSR_ACC,
         operation::ROL,
         operation::ROL_ACC,
         operation::ROR,
         operation::ROR_ACC,
         operation::SBC,
         operation::SEC};

     vector<operation> readInstrS{
         operation::PLA,
         operation::PLP,
         operation::RTI,
         operation::RTS,
         operation::TSX};

     vector<operation> writeInstrS{
         operation::BRK,
         operation::JSR,
         operation::PHA,
         operation::PHP,
         operation::TXS};

     vector<operation> readInstrPC{
         operation::BRK,
         operation::JSR,
     };

     vector<operation> writeInstrPC{
         operation::BCC,
         operation::BCS,
         operation::BEQ,
         operation::BMI,
         operation::BNE,
         operation::BPL,
         operation::BRK,
         operation::BVC,
         operation::BVS,
         operation::JMP,
         operation::JSR,
         operation::RTI,
         operation::RTS};

public:
     explicit exec_mos6502(i_memory_access *i_memory_access);
     void Reset();
     void Run(int32_t cycles, uint64_t &cycleCount, CycleMethod cycleMethod = CYCLE_COUNT);
     void NMI();
     void IRQ();
     void SetP(uint8_t value);
     void SetA(uint8_t value);
     void SetX(uint8_t value);
     void SetY(uint8_t value);
     void SetPC(uint16_t value);
     void StackPush(uint8_t byte);
     uint8_t StackPop();
     bool isAddrImm() const;
     bool isIllegalInstr() const;
     bool isCallInstr() const;
     bool isReturnInstr() const;
     bool isReadInstrA() const;
     bool isWriteInstrA() const;
     bool isReadInstrX() const;
     bool isWriteInstrX() const;
     bool isReadInstrY() const;
     bool isWriteInstrY() const;
     bool isReadInstrNegative() const;
     bool isWriteInstrNegative() const;
     bool isReadInstrOverflow() const;
     bool isWriteInstrOverflow() const;
     bool isReadInstrConstant() const;
     bool isWriteInstrConstant() const;
     bool isReadInstrBreak() const;
     bool isWriteInstrBreak() const;
     bool isReadInstrDecimal() const;
     bool isWriteInstrDecimal() const;
     bool isReadInstrInterrupt() const;
     bool isWriteInstrInterrupt() const;
     bool isReadInstrZero() const;
     bool isWriteInstrZero() const;
     bool isReadInstrCarry() const;
     bool isWriteInstrCarry() const;
     bool isReadInstrS() const;
     bool isWriteInstrS() const;
     bool isReadInstrPC() const;
     bool isWriteInstrPC() const;
     void forceRts();
     void forceJmp(uint16_t address);
     void forceJsr(uint16_t address);
};
