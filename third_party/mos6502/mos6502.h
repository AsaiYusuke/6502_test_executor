//============================================================================
// Name        : mos6502
// Author      : Gianluca Ghettini
// Version     : 1.0
// Copyright   :
// Description : A MOS 6502 CPU emulator written in C++
//============================================================================
#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>
#include "emulation/memory_access.h"
using namespace std;

#define NEGATIVE  0x80
#define OVERFLOW  0x40
#define CONSTANT  0x20
#define BREAK     0x10
#define DECIMAL   0x08
#define INTERRUPT 0x04
#define ZERO      0x02
#define CARRY     0x01

#define SET_NEGATIVE(x) (x ? (status |= NEGATIVE) : (status &= (~NEGATIVE)) )
#define SET_OVERFLOW(x) (x ? (status |= OVERFLOW) : (status &= (~OVERFLOW)) )
//#define SET_CONSTANT(x) (x ? (status |= CONSTANT) : (status &= (~CONSTANT)) )
//#define SET_BREAK(x) (x ? (status |= BREAK) : (status &= (~BREAK)) )
#define SET_DECIMAL(x) (x ? (status |= DECIMAL) : (status &= (~DECIMAL)) )
#define SET_INTERRUPT(x) (x ? (status |= INTERRUPT) : (status &= (~INTERRUPT)) )
#define SET_ZERO(x) (x ? (status |= ZERO) : (status &= (~ZERO)) )
#define SET_CARRY(x) (x ? (status |= CARRY) : (status &= (~CARRY)) )

#define IF_NEGATIVE() ((status & NEGATIVE) ? true : false)
#define IF_OVERFLOW() ((status & OVERFLOW) ? true : false)
#define IF_CONSTANT() ((status & CONSTANT) ? true : false)
#define IF_BREAK() ((status & BREAK) ? true : false)
#define IF_DECIMAL() ((status & DECIMAL) ? true : false)
#define IF_INTERRUPT() ((status & INTERRUPT) ? true : false)
#define IF_ZERO() ((status & ZERO) ? true : false)
#define IF_CARRY() ((status & CARRY) ? true : false)



class mos6502
{
private:
    // register reset values
    uint8_t reset_A = 0x00;
    uint8_t reset_X = 0x00;
    uint8_t reset_Y = 0x00;
    uint8_t reset_sp = 0xFD;
    uint8_t reset_status = CONSTANT;

	// registers
	uint8_t A; // accumulator
	uint8_t X; // X-index
	uint8_t Y; // Y-index

	// stack pointer
	uint8_t sp;

	// program counter
	uint16_t pc;

	// status register
	uint8_t status;

	typedef void (mos6502::*CodeExec)(uint16_t);
	typedef uint16_t (mos6502::*AddrExec)();

	struct Instr
	{
		AddrExec addr;
		CodeExec code;
		uint8_t cycles;
	};

	Instr InstrTable[256];

	void Exec(Instr i);

	bool illegalOpcode;

	// addressing modes
	uint16_t Addr_ACC(); // ACCUMULATOR
	uint16_t Addr_IMM(); // IMMEDIATE
	uint16_t Addr_ABS(); // ABSOLUTE
	uint16_t Addr_ZER(); // ZERO PAGE
	uint16_t Addr_ZEX(); // INDEXED-X ZERO PAGE
	uint16_t Addr_ZEY(); // INDEXED-Y ZERO PAGE
	uint16_t Addr_ABX(); // INDEXED-X ABSOLUTE
	uint16_t Addr_ABY(); // INDEXED-Y ABSOLUTE
	uint16_t Addr_IMP(); // IMPLIED
	uint16_t Addr_REL(); // RELATIVE
	uint16_t Addr_INX(); // INDEXED-X INDIRECT
	uint16_t Addr_INY(); // INDEXED-Y INDIRECT
	uint16_t Addr_ABI(); // ABSOLUTE INDIRECT

	// opcodes (grouped as per datasheet)
	void Op_ADC(uint16_t src);
	void Op_AND(uint16_t src);
	void Op_ASL(uint16_t src); 	void Op_ASL_ACC(uint16_t src);
	void Op_BCC(uint16_t src);
	void Op_BCS(uint16_t src);

	void Op_BEQ(uint16_t src);
	void Op_BIT(uint16_t src);
	void Op_BMI(uint16_t src);
	void Op_BNE(uint16_t src);
	void Op_BPL(uint16_t src);

	void Op_BRK(uint16_t src);
	void Op_BVC(uint16_t src);
	void Op_BVS(uint16_t src);
	void Op_CLC(uint16_t src);
	void Op_CLD(uint16_t src);

	void Op_CLI(uint16_t src);
	void Op_CLV(uint16_t src);
	void Op_CMP(uint16_t src);
	void Op_CPX(uint16_t src);
	void Op_CPY(uint16_t src);

	void Op_DEC(uint16_t src);
	void Op_DEX(uint16_t src);
	void Op_DEY(uint16_t src);
	void Op_EOR(uint16_t src);
	void Op_INC(uint16_t src);

	void Op_INX(uint16_t src);
	void Op_INY(uint16_t src);
	void Op_JMP(uint16_t src);
	void Op_JSR(uint16_t src);
	void Op_LDA(uint16_t src);

	void Op_LDX(uint16_t src);
	void Op_LDY(uint16_t src);
	void Op_LSR(uint16_t src); 	void Op_LSR_ACC(uint16_t src);
	void Op_NOP(uint16_t src);
	void Op_ORA(uint16_t src);

	void Op_PHA(uint16_t src);
	void Op_PHP(uint16_t src);
	void Op_PLA(uint16_t src);
	void Op_PLP(uint16_t src);
	void Op_ROL(uint16_t src); 	void Op_ROL_ACC(uint16_t src);

	void Op_ROR(uint16_t src);	void Op_ROR_ACC(uint16_t src);
	void Op_RTI(uint16_t src);
	void Op_RTS(uint16_t src);
	void Op_SBC(uint16_t src);
	void Op_SEC(uint16_t src);
	void Op_SED(uint16_t src);

	void Op_SEI(uint16_t src);
	void Op_STA(uint16_t src);
	void Op_STX(uint16_t src);
	void Op_STY(uint16_t src);
	void Op_TAX(uint16_t src);

	void Op_TAY(uint16_t src);
	void Op_TSX(uint16_t src);
	void Op_TXA(uint16_t src);
	void Op_TXS(uint16_t src);
	void Op_TYA(uint16_t src);

	void Op_ILLEGAL(uint16_t src);

	// IRQ, reset, NMI vectors
	static const uint16_t irqVectorH = 0xFFFF;
	static const uint16_t irqVectorL = 0xFFFE;
	static const uint16_t rstVectorH = 0xFFFD;
	static const uint16_t rstVectorL = 0xFFFC;
	static const uint16_t nmiVectorH = 0xFFFB;
	static const uint16_t nmiVectorL = 0xFFFA;

	// // read/write callbacks
	// typedef void (*BusWrite)(uint16_t, uint8_t);
	// typedef uint8_t (*BusRead)(uint16_t);
	// BusRead Read;
	// BusWrite Write;

	// // stack operations
	// inline void StackPush(uint8_t byte);
	// inline uint8_t StackPop();

	i_memory_access *memory_access;

	vector<CodeExec> callInstr{
		&mos6502::Op_BRK,
		&mos6502::Op_JSR
	};
	vector<CodeExec> returnInstr{
		&mos6502::Op_RTI,
		&mos6502::Op_RTS
	};
	vector<CodeExec> readInstrA{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_BIT,
		&mos6502::Op_CMP,
		&mos6502::Op_EOR,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ORA,
		&mos6502::Op_PHA,
		&mos6502::Op_PLA,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_STA,
		&mos6502::Op_TAX,
		&mos6502::Op_TAY
	};
	vector<CodeExec> writeInstrA{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_EOR,
		&mos6502::Op_LDA,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ORA,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_TXA,
		&mos6502::Op_TYA
	};
	vector<CodeExec> readInstrX{
		&mos6502::Op_CPX,
		&mos6502::Op_DEX,
		&mos6502::Op_INX,
		&mos6502::Op_STX,
		&mos6502::Op_TXA,
		&mos6502::Op_TXS
	};
	vector<CodeExec> writeInstrX{
		&mos6502::Op_DEX,
		&mos6502::Op_INX,
		&mos6502::Op_LDX,
		&mos6502::Op_TAX,
		&mos6502::Op_TSX
	};
	vector<CodeExec> readInstrY{
		&mos6502::Op_CPY,
		&mos6502::Op_DEY,
		&mos6502::Op_INY,
		&mos6502::Op_STY,
		&mos6502::Op_TYA
	};
	vector<CodeExec> writeInstrY{
		&mos6502::Op_DEY,
		&mos6502::Op_INY,
		&mos6502::Op_LDY,
		&mos6502::Op_TAY
	};
	vector<CodeExec> readInstrNegative{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_BIT,
		&mos6502::Op_BMI,
		&mos6502::Op_BPL,
		&mos6502::Op_CMP,
		&mos6502::Op_CPX,
		&mos6502::Op_CPY,
		&mos6502::Op_DEC,
		&mos6502::Op_DEX,
		&mos6502::Op_DEY,
		&mos6502::Op_EOR,
		&mos6502::Op_INC,
		&mos6502::Op_INX,
		&mos6502::Op_INY,
		&mos6502::Op_LDA,
		&mos6502::Op_LDX,
		&mos6502::Op_LDY,
		&mos6502::Op_LSR,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ORA,
		&mos6502::Op_PLA,
		&mos6502::Op_ROL,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_TAX,
		&mos6502::Op_TAY,
		&mos6502::Op_TSX,
		&mos6502::Op_TXA,
		&mos6502::Op_TYA
	};
	vector<CodeExec> readInstrOverflow{
		&mos6502::Op_ADC,
		&mos6502::Op_BVC,
		&mos6502::Op_BVS,
		&mos6502::Op_CLV,
		&mos6502::Op_SBC
	};
	vector<CodeExec> readInstrConstant{
	};
	vector<CodeExec> readInstrBreak{
	};
	vector<CodeExec> readInstrDecimal{
		&mos6502::Op_ADC,
		&mos6502::Op_CLD,
		&mos6502::Op_SBC,
		&mos6502::Op_SED
	};
	vector<CodeExec> readInstrInterrupt{
		&mos6502::Op_BRK,
		&mos6502::Op_CLI,
		&mos6502::Op_SEI
	};
	vector<CodeExec> readInstrZero{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_BEQ,
		&mos6502::Op_BIT,
		&mos6502::Op_BNE,
		&mos6502::Op_CMP,
		&mos6502::Op_CPX,
		&mos6502::Op_CPY,
		&mos6502::Op_DEC,
		&mos6502::Op_DEX,
		&mos6502::Op_DEY,
		&mos6502::Op_EOR,
		&mos6502::Op_INC,
		&mos6502::Op_INX,
		&mos6502::Op_INY,
		&mos6502::Op_LDA,
		&mos6502::Op_LDX,
		&mos6502::Op_LDY,
		&mos6502::Op_LSR,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ORA,
		&mos6502::Op_PHA,
		&mos6502::Op_PLA,
		&mos6502::Op_ROL,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_TAX,
		&mos6502::Op_TAY,
		&mos6502::Op_TSX,
		&mos6502::Op_TXA,
		&mos6502::Op_TYA
	};
	vector<CodeExec> readInstrCarry{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_BCC,
		&mos6502::Op_BCS,
		&mos6502::Op_CLC,
		&mos6502::Op_CMP,
		&mos6502::Op_CPX,
		&mos6502::Op_CPY,
		&mos6502::Op_LSR,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ROL,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_SEC
	};
	vector<CodeExec> writeInstrNegative{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_BIT,
		&mos6502::Op_CMP,
		&mos6502::Op_CPX,
		&mos6502::Op_CPY,
		&mos6502::Op_DEC,
		&mos6502::Op_DEX,
		&mos6502::Op_DEY,
		&mos6502::Op_EOR,
		&mos6502::Op_INC,
		&mos6502::Op_INX,
		&mos6502::Op_INY,
		&mos6502::Op_LDA,
		&mos6502::Op_LDX,
		&mos6502::Op_LDY,
		&mos6502::Op_LSR,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ORA,
		&mos6502::Op_PLA,
		&mos6502::Op_ROL,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_TAX,
		&mos6502::Op_TAY,
		&mos6502::Op_TSX,
		&mos6502::Op_TXA,
		&mos6502::Op_TYA
	};
	vector<CodeExec> writeInstrOverflow{
		&mos6502::Op_ADC,
		&mos6502::Op_CLV,
		&mos6502::Op_SBC
	};
	vector<CodeExec> writeInstrConstant{
	};
	vector<CodeExec> writeInstrBreak{
	};
	vector<CodeExec> writeInstrDecimal{
		&mos6502::Op_CLD,
		&mos6502::Op_SED
	};
	vector<CodeExec> writeInstrInterrupt{
		&mos6502::Op_BRK,
		&mos6502::Op_CLI,
		&mos6502::Op_SEI
	};
	vector<CodeExec> writeInstrZero{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_BIT,
		&mos6502::Op_CMP,
		&mos6502::Op_CPX,
		&mos6502::Op_CPY,
		&mos6502::Op_DEC,
		&mos6502::Op_DEX,
		&mos6502::Op_DEY,
		&mos6502::Op_EOR,
		&mos6502::Op_INC,
		&mos6502::Op_INX,
		&mos6502::Op_INY,
		&mos6502::Op_LDA,
		&mos6502::Op_LDX,
		&mos6502::Op_LDY,
		&mos6502::Op_LSR,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ORA,
		&mos6502::Op_PHA,
		&mos6502::Op_PLA,
		&mos6502::Op_ROL,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_TAX,
		&mos6502::Op_TAY,
		&mos6502::Op_TSX,
		&mos6502::Op_TXA,
		&mos6502::Op_TYA
	};
	vector<CodeExec> writeInstrCarry{
		&mos6502::Op_ADC,
		&mos6502::Op_AND,
		&mos6502::Op_ASL,
		&mos6502::Op_ASL_ACC,
		&mos6502::Op_CLC,
		&mos6502::Op_CMP,
		&mos6502::Op_CPX,
		&mos6502::Op_CPY,
		&mos6502::Op_LSR,
		&mos6502::Op_LSR_ACC,
		&mos6502::Op_ROL,
		&mos6502::Op_ROL_ACC,
		&mos6502::Op_ROR,
		&mos6502::Op_ROR_ACC,
		&mos6502::Op_SBC,
		&mos6502::Op_SEC
	};
	vector<CodeExec> readInstrS{
		&mos6502::Op_PLA,
		&mos6502::Op_PLP,
		&mos6502::Op_RTI,
		&mos6502::Op_RTS,
		&mos6502::Op_TSX
	};
	vector<CodeExec> writeInstrS{
		&mos6502::Op_BRK,
		&mos6502::Op_JSR,
		&mos6502::Op_PHA,
		&mos6502::Op_PHP,
		&mos6502::Op_TXS
	};
	vector<CodeExec> readInstrPC{
		&mos6502::Op_BRK,
		&mos6502::Op_JSR,
	};
	vector<CodeExec> writeInstrPC{
		&mos6502::Op_BCC,
		&mos6502::Op_BCS,
		&mos6502::Op_BEQ,
		&mos6502::Op_BMI,
		&mos6502::Op_BNE,
		&mos6502::Op_BPL,
		&mos6502::Op_BRK,
		&mos6502::Op_BVC,
		&mos6502::Op_BVS,
		&mos6502::Op_JMP,
		&mos6502::Op_JSR,
		&mos6502::Op_RTI,
		&mos6502::Op_RTS
	};

public:
	enum CycleMethod {
		INST_COUNT,
		CYCLE_COUNT,
	};
    mos6502(i_memory_access *i_memory_access);
	void NMI();
	void IRQ();
	void Reset();
	void Run(
		int32_t cycles,
		uint64_t& cycleCount,
		CycleMethod cycleMethod = CYCLE_COUNT);
    uint16_t GetPC();
    uint8_t GetS();
    uint8_t GetP();
    uint8_t GetA();
    uint8_t GetX();
    uint8_t GetY();
    void SetResetS(uint8_t value);
    void SetResetP(uint8_t value);
    void SetResetA(uint8_t value);
    void SetResetX(uint8_t value);
    void SetResetY(uint8_t value);
    uint8_t GetResetS();
    uint8_t GetResetP();
    uint8_t GetResetA();
    uint8_t GetResetX();
    uint8_t GetResetY();

    void SetP(uint8_t value);
    void SetA(uint8_t value);
    void SetX(uint8_t value);
    void SetY(uint8_t value);
	void SetPC(uint16_t value);
	void StackPush(uint8_t byte);
	uint8_t StackPop();
	bool isIllegalInstr();
	bool isCallInstr();
	bool isReturnInstr();
	bool isReadInstrA();
	bool isWriteInstrA();
	bool isReadInstrX();
	bool isWriteInstrX();
	bool isReadInstrY();
	bool isWriteInstrY();
	bool isReadInstrNegative();
	bool isWriteInstrNegative();
	bool isReadInstrOverflow();
	bool isWriteInstrOverflow();
	bool isReadInstrConstant();
	bool isWriteInstrConstant();
	bool isReadInstrBreak();
	bool isWriteInstrBreak();
	bool isReadInstrDecimal();
	bool isWriteInstrDecimal();
	bool isReadInstrInterrupt();
	bool isWriteInstrInterrupt();
	bool isReadInstrZero();
	bool isWriteInstrZero();
	bool isReadInstrCarry();
	bool isWriteInstrCarry();
	bool isReadInstrS();
	bool isWriteInstrS();
	bool isReadInstrPC();
	bool isWriteInstrPC();
	void forceRts();
	void forceJmp(uint16_t address);
	void forceJsr(uint16_t address);
};
