#include "emulation/mos6502/exec_mos6502.hpp"

#include <algorithm>
#include <mutex>
#include <stdexcept>

namespace
{
using mos6502_metadata::addressing_mode;
using mos6502_metadata::operation;

size_t readsBeforeOperandData(addressing_mode mode)
{
    switch (mode)
    {
    case addressing_mode::IMM:
        return 0;
    case addressing_mode::ZER:
    case addressing_mode::ZEX:
    case addressing_mode::ZEY:
    case addressing_mode::REL:
        return 1;
    case addressing_mode::ABS:
    case addressing_mode::ABX:
    case addressing_mode::ABY:
    case addressing_mode::ABI:
        return 2;
    case addressing_mode::INX:
    case addressing_mode::INY:
        return 3;
    case addressing_mode::ACC:
    case addressing_mode::IMP:
        return 0;
    }

    throw logic_error("Invalid MOS 6502 addressing mode");
}

bool readsMemoryOperand(operation op)
{
    switch (op)
    {
    case operation::ADC:
    case operation::AND:
    case operation::ASL:
    case operation::BIT:
    case operation::CMP:
    case operation::CPX:
    case operation::CPY:
    case operation::DEC:
    case operation::EOR:
    case operation::INC:
    case operation::LDA:
    case operation::LDX:
    case operation::LDY:
    case operation::LSR:
    case operation::ORA:
    case operation::ROL:
    case operation::ROR:
    case operation::SBC:
        return true;
    default:
        return false;
    }
}

bool writesMemoryOperand(operation op)
{
    switch (op)
    {
    case operation::ASL:
    case operation::DEC:
    case operation::INC:
    case operation::LSR:
    case operation::ROL:
    case operation::ROR:
    case operation::STA:
    case operation::STX:
    case operation::STY:
        return true;
    default:
        return false;
    }
}

void initializeMos6502InstructionTable()
{
    static once_flag initialized;
    call_once(initialized, [] {
        mos6502 cpu(nullptr, nullptr);
    });
}

class scoped_value
{
public:
    explicit scoped_value(bool &value) : value(value), previous(value)
    {
    }

    ~scoped_value()
    {
        value = previous;
    }

private:
    bool &value;
    bool previous;
};

class scoped_cpu
{
public:
    scoped_cpu(exec_mos6502 *&slot, exec_mos6502 *value) : slot(slot), previous(slot)
    {
        slot = value;
    }

    ~scoped_cpu()
    {
        slot = previous;
    }

private:
    exec_mos6502 *&slot;
    exec_mos6502 *previous;
};
}

thread_local exec_mos6502 *exec_mos6502::active_cpu = nullptr;

exec_mos6502::exec_mos6502(i_memory_access *i_memory_access)
    : mos6502((initializeMos6502InstructionTable(), &exec_mos6502::readBus),
              &exec_mos6502::writeBus),
      memory_access(i_memory_access)
{
    if (memory_access == nullptr)
        throw invalid_argument("MOS 6502 requires memory access");
}

uint8_t exec_mos6502::readBus(uint16_t address)
{
    if (active_cpu == nullptr)
        throw logic_error("MOS 6502 bus read without an active CPU");

    return active_cpu->readBusValue(address);
}

void exec_mos6502::writeBus(uint16_t address, uint8_t value)
{
    if (active_cpu == nullptr)
        throw logic_error("MOS 6502 bus write without an active CPU");

    active_cpu->writeBusValue(address, value);
}

uint8_t exec_mos6502::readBusValue(uint16_t address)
{
    if (reset_access)
        return memory_access->read_raw(address);

    if (!instruction_running)
        return memory_access->read(address);

    if (!opcode_fetch_seen)
    {
        opcode_fetch_seen = true;
        return memory_access->read(address);
    }

    auto reads_operand = readsMemoryOperand(active_instruction.op);
    auto is_operand_data =
        active_instruction.op == operation::BRK ||
        (reads_operand &&
         instruction_reads == readsBeforeOperandData(active_instruction.mode));
    ++instruction_reads;

    return is_operand_data
        ? memory_access->read_data(address)
        : memory_access->read(address);
}

void exec_mos6502::writeBusValue(uint16_t address, uint8_t value)
{
    if (instruction_running && writesMemoryOperand(active_instruction.op))
        memory_access->write_data(address, value);
    else
        memory_access->write(address, value);
}

mos6502_metadata::instruction exec_mos6502::instructionAtPC() const
{
    auto pc = const_cast<exec_mos6502 *>(this)->GetPC();
    return mos6502_metadata::decode(memory_access->read_raw(pc));
}

bool exec_mos6502::contains(const vector<operation> &instructions) const
{
    return find(instructions.begin(), instructions.end(), instructionAtPC().op) !=
        instructions.end();
}

void exec_mos6502::Reset()
{
    scoped_cpu binding(active_cpu, this);
    scoped_value reset_binding(reset_access);
    reset_access = true;
    mos6502::Reset();
    illegal_instruction = false;
}

void exec_mos6502::Run(int32_t cycles, uint64_t &cycleCount, CycleMethod cycleMethod)
{
    scoped_cpu binding(active_cpu, this);
    scoped_value running_binding(instruction_running);
    instruction_running = true;
    opcode_fetch_seen = false;
    instruction_reads = 0;
    active_instruction = instructionAtPC();
    is_addr_imm = active_instruction.mode == addressing_mode::IMM;
    illegal_instruction = active_instruction.op == operation::invalid;
    mos6502::Run(cycles, cycleCount, cycleMethod);
}

void exec_mos6502::NMI()
{
    StackPush(static_cast<uint8_t>(GetPC() >> 8));
    StackPush(static_cast<uint8_t>(GetPC()));
    StackPush(GetP() | CONSTANT | BREAK);
    SetP(GetP() | INTERRUPT);

    auto low = memory_access->read(0xFFFA);
    auto high = memory_access->read(0xFFFB);
    SetPC(static_cast<uint16_t>((high << 8) | low));
}

void exec_mos6502::IRQ()
{
    if ((GetP() & INTERRUPT) != 0)
        return;

    StackPush(static_cast<uint8_t>(GetPC() >> 8));
    StackPush(static_cast<uint8_t>(GetPC()));
    StackPush(GetP() | CONSTANT | BREAK);
    SetP(GetP() | INTERRUPT);

    auto low = memory_access->read(0xFFFE);
    auto high = memory_access->read(0xFFFF);
    SetPC(static_cast<uint16_t>((high << 8) | low));
}

void exec_mos6502::SetP(uint8_t value)
{
    mos6502::SetP(value);
}

void exec_mos6502::SetA(uint8_t value)
{
    mos6502::SetA(value);
}

void exec_mos6502::SetX(uint8_t value)
{
    mos6502::SetX(value);
}

void exec_mos6502::SetY(uint8_t value)
{
    mos6502::SetY(value);
}

void exec_mos6502::SetPC(uint16_t value)
{
    mos6502::SetPC(value);
}

void exec_mos6502::StackPush(uint8_t byte)
{
    auto stack_pointer = GetS();
    memory_access->write(static_cast<uint16_t>(0x0100 | stack_pointer), byte);
    SetS(static_cast<uint8_t>(stack_pointer - 1));
}

uint8_t exec_mos6502::StackPop()
{
    auto stack_pointer = static_cast<uint8_t>(GetS() + 1);
    SetS(stack_pointer);
    return memory_access->read(static_cast<uint16_t>(0x0100 | stack_pointer));
}

bool exec_mos6502::isAddrImm() const
{
    return is_addr_imm;
}

bool exec_mos6502::isIllegalInstr() const
{
    return illegal_instruction;
}

bool exec_mos6502::isCallInstr() const
{
    return contains(callInstr);
}

bool exec_mos6502::isReturnInstr() const
{
    return contains(returnInstr);
}

bool exec_mos6502::isReadInstrA() const
{
    return contains(readInstrA);
}

bool exec_mos6502::isWriteInstrA() const
{
    return contains(writeInstrA);
}

bool exec_mos6502::isReadInstrX() const
{
    return contains(readInstrX);
}

bool exec_mos6502::isWriteInstrX() const
{
    return contains(writeInstrX);
}

bool exec_mos6502::isReadInstrY() const
{
    return contains(readInstrY);
}

bool exec_mos6502::isWriteInstrY() const
{
    return contains(writeInstrY);
}

bool exec_mos6502::isReadInstrNegative() const
{
    return contains(readInstrNegative);
}

bool exec_mos6502::isWriteInstrNegative() const
{
    return contains(writeInstrNegative);
}

bool exec_mos6502::isReadInstrOverflow() const
{
    return contains(readInstrOverflow);
}

bool exec_mos6502::isWriteInstrOverflow() const
{
    return contains(writeInstrOverflow);
}

bool exec_mos6502::isReadInstrConstant() const
{
    return contains(readInstrConstant);
}

bool exec_mos6502::isWriteInstrConstant() const
{
    return contains(writeInstrConstant);
}

bool exec_mos6502::isReadInstrBreak() const
{
    return contains(readInstrBreak);
}

bool exec_mos6502::isWriteInstrBreak() const
{
    return contains(writeInstrBreak);
}

bool exec_mos6502::isReadInstrDecimal() const
{
    return contains(readInstrDecimal);
}

bool exec_mos6502::isWriteInstrDecimal() const
{
    return contains(writeInstrDecimal);
}

bool exec_mos6502::isReadInstrInterrupt() const
{
    return contains(readInstrInterrupt);
}

bool exec_mos6502::isWriteInstrInterrupt() const
{
    return contains(writeInstrInterrupt);
}

bool exec_mos6502::isReadInstrZero() const
{
    return contains(readInstrZero);
}

bool exec_mos6502::isWriteInstrZero() const
{
    return contains(writeInstrZero);
}

bool exec_mos6502::isReadInstrCarry() const
{
    return contains(readInstrCarry);
}

bool exec_mos6502::isWriteInstrCarry() const
{
    return contains(writeInstrCarry);
}

bool exec_mos6502::isReadInstrS() const
{
    return contains(readInstrS);
}

bool exec_mos6502::isWriteInstrS() const
{
    return contains(writeInstrS);
}

bool exec_mos6502::isReadInstrPC() const
{
    return contains(readInstrPC);
}

bool exec_mos6502::isWriteInstrPC() const
{
    return contains(writeInstrPC);
}

void exec_mos6502::forceRts()
{
    auto low = StackPop();
    auto high = StackPop();
    SetPC(static_cast<uint16_t>(((high << 8) | low) + 1));
}

void exec_mos6502::forceJmp(uint16_t address)
{
    SetPC(address);
}

void exec_mos6502::forceJsr(uint16_t address)
{
    auto return_address = static_cast<uint16_t>(GetPC() - 1);
    StackPush(static_cast<uint8_t>(return_address >> 8));
    StackPush(static_cast<uint8_t>(return_address));
    SetPC(address);
}
