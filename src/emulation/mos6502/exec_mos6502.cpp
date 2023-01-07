#include "emulation/mos6502/exec_mos6502.hpp"

exec_mos6502::exec_mos6502(i_memory_access *i_memory_access) : mos6502(i_memory_access)
{
}

void exec_mos6502::Exec(Instr i)
{
	is_addr_imm = i.addr == &exec_mos6502::Addr_IMM;
	mos6502::Exec(i);
}

void exec_mos6502::SetP(uint8_t value)
{
	status = value;
}

void exec_mos6502::SetA(uint8_t value)
{
	A = value;
}

void exec_mos6502::SetX(uint8_t value)
{
	X = value;
}

void exec_mos6502::SetY(uint8_t value)
{
	Y = value;
}

void exec_mos6502::SetPC(uint16_t value)
{
	pc = value;
}

void exec_mos6502::StackPush(uint8_t byte)
{
	mos6502::StackPush(byte);
}

uint8_t exec_mos6502::StackPop()
{
	return mos6502::StackPop();
}

bool exec_mos6502::isAddrImm() const
{
	return is_addr_imm;
}

bool exec_mos6502::isIllegalInstr() const
{
	return illegalOpcode;
}

bool exec_mos6502::isCallInstr() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(callInstr.begin(), callInstr.end(), instr) != callInstr.end();
}

bool exec_mos6502::isReturnInstr() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(returnInstr.begin(), returnInstr.end(), instr) != returnInstr.end();
}

bool exec_mos6502::isReadInstrA() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrA.begin(), readInstrA.end(), instr) != readInstrA.end();
}

bool exec_mos6502::isWriteInstrA() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrA.begin(), writeInstrA.end(), instr) != writeInstrA.end();
}

bool exec_mos6502::isReadInstrX() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrX.begin(), readInstrX.end(), instr) != readInstrX.end();
}

bool exec_mos6502::isWriteInstrX() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrX.begin(), writeInstrX.end(), instr) != writeInstrX.end();
}

bool exec_mos6502::isReadInstrY() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrY.begin(), readInstrY.end(), instr) != readInstrY.end();
}

bool exec_mos6502::isWriteInstrY() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrY.begin(), writeInstrY.end(), instr) != writeInstrY.end();
}

bool exec_mos6502::isReadInstrNegative() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrNegative.begin(), readInstrNegative.end(), instr) != readInstrNegative.end();
}

bool exec_mos6502::isWriteInstrNegative() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrNegative.begin(), writeInstrNegative.end(), instr) != writeInstrNegative.end();
}

bool exec_mos6502::isReadInstrOverflow() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrOverflow.begin(), readInstrOverflow.end(), instr) != readInstrOverflow.end();
}

bool exec_mos6502::isWriteInstrOverflow() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrOverflow.begin(), writeInstrOverflow.end(), instr) != writeInstrOverflow.end();
}

bool exec_mos6502::isReadInstrConstant() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrConstant.begin(), readInstrConstant.end(), instr) != readInstrConstant.end();
}

bool exec_mos6502::isWriteInstrConstant() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrConstant.begin(), writeInstrConstant.end(), instr) != writeInstrConstant.end();
}

bool exec_mos6502::isReadInstrBreak() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrBreak.begin(), readInstrBreak.end(), instr) != readInstrBreak.end();
}

bool exec_mos6502::isWriteInstrBreak() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrBreak.begin(), writeInstrBreak.end(), instr) != writeInstrBreak.end();
}

bool exec_mos6502::isReadInstrDecimal() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrDecimal.begin(), readInstrDecimal.end(), instr) != readInstrDecimal.end();
}

bool exec_mos6502::isWriteInstrDecimal() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrDecimal.begin(), writeInstrDecimal.end(), instr) != writeInstrDecimal.end();
}

bool exec_mos6502::isReadInstrInterrupt() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrInterrupt.begin(), readInstrInterrupt.end(), instr) != readInstrInterrupt.end();
}

bool exec_mos6502::isWriteInstrInterrupt() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrInterrupt.begin(), writeInstrInterrupt.end(), instr) != writeInstrInterrupt.end();
}

bool exec_mos6502::isReadInstrZero() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrZero.begin(), readInstrZero.end(), instr) != readInstrZero.end();
}

bool exec_mos6502::isWriteInstrZero() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrZero.begin(), writeInstrZero.end(), instr) != writeInstrZero.end();
}

bool exec_mos6502::isReadInstrCarry() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrCarry.begin(), readInstrCarry.end(), instr) != readInstrCarry.end();
}

bool exec_mos6502::isWriteInstrCarry() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrCarry.begin(), writeInstrCarry.end(), instr) != writeInstrCarry.end();
}

bool exec_mos6502::isReadInstrS() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrS.begin(), readInstrS.end(), instr) != readInstrS.end();
}

bool exec_mos6502::isWriteInstrS() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrS.begin(), writeInstrS.end(), instr) != writeInstrS.end();
}

bool exec_mos6502::isReadInstrPC() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(readInstrPC.begin(), readInstrPC.end(), instr) != readInstrPC.end();
}

bool exec_mos6502::isWriteInstrPC() const
{
	auto instr = InstrTable[memory_access->read_raw(pc)].code;
	return find(writeInstrPC.begin(), writeInstrPC.end(), instr) != writeInstrPC.end();
}

void exec_mos6502::forceRts()
{
	Op_RTS(0);
}

void exec_mos6502::forceJmp(uint16_t address)
{
	Op_JMP(address);
}

void exec_mos6502::forceJsr(uint16_t address)
{
	Op_JSR(address);
}
