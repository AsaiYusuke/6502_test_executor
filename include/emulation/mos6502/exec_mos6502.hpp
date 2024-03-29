#pragma once

#include <vector>

#include "mos6502.h"

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
     vector<CodeExec> callInstr{
         &exec_mos6502::Op_BRK,
         &exec_mos6502::Op_JSR};

     vector<CodeExec> returnInstr{
         &exec_mos6502::Op_RTI,
         &exec_mos6502::Op_RTS};

     vector<CodeExec> readInstrA{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_BIT,
         &exec_mos6502::Op_CMP,
         &exec_mos6502::Op_EOR,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ORA,
         &exec_mos6502::Op_PHA,
         &exec_mos6502::Op_PLA,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_STA,
         &exec_mos6502::Op_TAX,
         &exec_mos6502::Op_TAY};

     vector<CodeExec> writeInstrA{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_EOR,
         &exec_mos6502::Op_LDA,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ORA,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_TXA,
         &exec_mos6502::Op_TYA};

     vector<CodeExec> readInstrX{
         &exec_mos6502::Op_CPX,
         &exec_mos6502::Op_DEX,
         &exec_mos6502::Op_INX,
         &exec_mos6502::Op_STX,
         &exec_mos6502::Op_TXA,
         &exec_mos6502::Op_TXS};

     vector<CodeExec> writeInstrX{
         &exec_mos6502::Op_DEX,
         &exec_mos6502::Op_INX,
         &exec_mos6502::Op_LDX,
         &exec_mos6502::Op_TAX,
         &exec_mos6502::Op_TSX};

     vector<CodeExec> readInstrY{
         &exec_mos6502::Op_CPY,
         &exec_mos6502::Op_DEY,
         &exec_mos6502::Op_INY,
         &exec_mos6502::Op_STY,
         &exec_mos6502::Op_TYA};

     vector<CodeExec> writeInstrY{
         &exec_mos6502::Op_DEY,
         &exec_mos6502::Op_INY,
         &exec_mos6502::Op_LDY,
         &exec_mos6502::Op_TAY};

     vector<CodeExec> readInstrNegative{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_BIT,
         &exec_mos6502::Op_BMI,
         &exec_mos6502::Op_BPL,
         &exec_mos6502::Op_CMP,
         &exec_mos6502::Op_CPX,
         &exec_mos6502::Op_CPY,
         &exec_mos6502::Op_DEC,
         &exec_mos6502::Op_DEX,
         &exec_mos6502::Op_DEY,
         &exec_mos6502::Op_EOR,
         &exec_mos6502::Op_INC,
         &exec_mos6502::Op_INX,
         &exec_mos6502::Op_INY,
         &exec_mos6502::Op_LDA,
         &exec_mos6502::Op_LDX,
         &exec_mos6502::Op_LDY,
         &exec_mos6502::Op_LSR,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ORA,
         &exec_mos6502::Op_PLA,
         &exec_mos6502::Op_ROL,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_TAX,
         &exec_mos6502::Op_TAY,
         &exec_mos6502::Op_TSX,
         &exec_mos6502::Op_TXA,
         &exec_mos6502::Op_TYA};

     vector<CodeExec> readInstrOverflow{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_BVC,
         &exec_mos6502::Op_BVS,
         &exec_mos6502::Op_CLV,
         &exec_mos6502::Op_SBC};

     vector<CodeExec> readInstrConstant{};

     vector<CodeExec> readInstrBreak{};

     vector<CodeExec> readInstrDecimal{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_CLD,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_SED};

     vector<CodeExec> readInstrInterrupt{
         &exec_mos6502::Op_BRK,
         &exec_mos6502::Op_CLI,
         &exec_mos6502::Op_SEI};

     vector<CodeExec> readInstrZero{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_BEQ,
         &exec_mos6502::Op_BIT,
         &exec_mos6502::Op_BNE,
         &exec_mos6502::Op_CMP,
         &exec_mos6502::Op_CPX,
         &exec_mos6502::Op_CPY,
         &exec_mos6502::Op_DEC,
         &exec_mos6502::Op_DEX,
         &exec_mos6502::Op_DEY,
         &exec_mos6502::Op_EOR,
         &exec_mos6502::Op_INC,
         &exec_mos6502::Op_INX,
         &exec_mos6502::Op_INY,
         &exec_mos6502::Op_LDA,
         &exec_mos6502::Op_LDX,
         &exec_mos6502::Op_LDY,
         &exec_mos6502::Op_LSR,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ORA,
         &exec_mos6502::Op_PHA,
         &exec_mos6502::Op_PLA,
         &exec_mos6502::Op_ROL,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_TAX,
         &exec_mos6502::Op_TAY,
         &exec_mos6502::Op_TSX,
         &exec_mos6502::Op_TXA,
         &exec_mos6502::Op_TYA};

     vector<CodeExec> readInstrCarry{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_BCC,
         &exec_mos6502::Op_BCS,
         &exec_mos6502::Op_CLC,
         &exec_mos6502::Op_CMP,
         &exec_mos6502::Op_CPX,
         &exec_mos6502::Op_CPY,
         &exec_mos6502::Op_LSR,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ROL,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_SEC};

     vector<CodeExec> writeInstrNegative{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_BIT,
         &exec_mos6502::Op_CMP,
         &exec_mos6502::Op_CPX,
         &exec_mos6502::Op_CPY,
         &exec_mos6502::Op_DEC,
         &exec_mos6502::Op_DEX,
         &exec_mos6502::Op_DEY,
         &exec_mos6502::Op_EOR,
         &exec_mos6502::Op_INC,
         &exec_mos6502::Op_INX,
         &exec_mos6502::Op_INY,
         &exec_mos6502::Op_LDA,
         &exec_mos6502::Op_LDX,
         &exec_mos6502::Op_LDY,
         &exec_mos6502::Op_LSR,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ORA,
         &exec_mos6502::Op_PLA,
         &exec_mos6502::Op_ROL,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_TAX,
         &exec_mos6502::Op_TAY,
         &exec_mos6502::Op_TSX,
         &exec_mos6502::Op_TXA,
         &exec_mos6502::Op_TYA};

     vector<CodeExec> writeInstrOverflow{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_CLV,
         &exec_mos6502::Op_SBC};

     vector<CodeExec> writeInstrConstant{};

     vector<CodeExec> writeInstrBreak{};

     vector<CodeExec> writeInstrDecimal{
         &exec_mos6502::Op_CLD,
         &exec_mos6502::Op_SED};

     vector<CodeExec> writeInstrInterrupt{
         &exec_mos6502::Op_BRK,
         &exec_mos6502::Op_CLI,
         &exec_mos6502::Op_SEI};

     vector<CodeExec> writeInstrZero{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_BIT,
         &exec_mos6502::Op_CMP,
         &exec_mos6502::Op_CPX,
         &exec_mos6502::Op_CPY,
         &exec_mos6502::Op_DEC,
         &exec_mos6502::Op_DEX,
         &exec_mos6502::Op_DEY,
         &exec_mos6502::Op_EOR,
         &exec_mos6502::Op_INC,
         &exec_mos6502::Op_INX,
         &exec_mos6502::Op_INY,
         &exec_mos6502::Op_LDA,
         &exec_mos6502::Op_LDX,
         &exec_mos6502::Op_LDY,
         &exec_mos6502::Op_LSR,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ORA,
         &exec_mos6502::Op_PHA,
         &exec_mos6502::Op_PLA,
         &exec_mos6502::Op_ROL,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_TAX,
         &exec_mos6502::Op_TAY,
         &exec_mos6502::Op_TSX,
         &exec_mos6502::Op_TXA,
         &exec_mos6502::Op_TYA};

     vector<CodeExec> writeInstrCarry{
         &exec_mos6502::Op_ADC,
         &exec_mos6502::Op_AND,
         &exec_mos6502::Op_ASL,
         &exec_mos6502::Op_ASL_ACC,
         &exec_mos6502::Op_CLC,
         &exec_mos6502::Op_CMP,
         &exec_mos6502::Op_CPX,
         &exec_mos6502::Op_CPY,
         &exec_mos6502::Op_LSR,
         &exec_mos6502::Op_LSR_ACC,
         &exec_mos6502::Op_ROL,
         &exec_mos6502::Op_ROL_ACC,
         &exec_mos6502::Op_ROR,
         &exec_mos6502::Op_ROR_ACC,
         &exec_mos6502::Op_SBC,
         &exec_mos6502::Op_SEC};

     vector<CodeExec> readInstrS{
         &exec_mos6502::Op_PLA,
         &exec_mos6502::Op_PLP,
         &exec_mos6502::Op_RTI,
         &exec_mos6502::Op_RTS,
         &exec_mos6502::Op_TSX};

     vector<CodeExec> writeInstrS{
         &exec_mos6502::Op_BRK,
         &exec_mos6502::Op_JSR,
         &exec_mos6502::Op_PHA,
         &exec_mos6502::Op_PHP,
         &exec_mos6502::Op_TXS};

     vector<CodeExec> readInstrPC{
         &exec_mos6502::Op_BRK,
         &exec_mos6502::Op_JSR,
     };

     vector<CodeExec> writeInstrPC{
         &exec_mos6502::Op_BCC,
         &exec_mos6502::Op_BCS,
         &exec_mos6502::Op_BEQ,
         &exec_mos6502::Op_BMI,
         &exec_mos6502::Op_BNE,
         &exec_mos6502::Op_BPL,
         &exec_mos6502::Op_BRK,
         &exec_mos6502::Op_BVC,
         &exec_mos6502::Op_BVS,
         &exec_mos6502::Op_JMP,
         &exec_mos6502::Op_JSR,
         &exec_mos6502::Op_RTI,
         &exec_mos6502::Op_RTS};

     bool is_addr_imm;

public:
     explicit exec_mos6502(i_memory_access *i_memory_access);
     void Exec(Instr i);
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
