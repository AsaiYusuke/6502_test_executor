#include "cpu_device.h"

cpu_device::cpu_device(i_memory_access *i_memory_access)
{
    cpu = new mos6502(i_memory_access);
}

void cpu_device::clear(uint16_t target_program_counter)
{
    cpu->Reset();
    cpu->setPC(target_program_counter);
    cpu->StackPush(0xFF);
    cpu->StackPush(0xFE);
}

bool cpu_device::execute()
{
    int count = 0;
    uint64_t cycleCount = 0;
    do
    {
        cpu->Run(1, cycleCount, cpu->INST_COUNT);
    } while (cpu->getPC() != 0xFFFF && ++count < timeout());

    return count < timeout();
}

int cpu_device::timeout()
{
    return DEFAULT_TIMEOUT;
}

uint16_t cpu_device::get_reg_pc()
{
    return cpu->getPC();
}

void cpu_device::set_reg_pc(uint16_t value)
{
    cpu->setPC(value);
}

uint8_t cpu_device::get_reg_a()
{
    return cpu->getA();
}

void cpu_device::set_reg_a(uint8_t value)
{
    cpu->setA(value);
}

uint8_t cpu_device::get_reg_x()
{
    return cpu->getX();
}

void cpu_device::set_reg_x(uint8_t value)
{
    cpu->setX(value);
}

uint8_t cpu_device::get_reg_y()
{
    return cpu->getY();
}

void cpu_device::set_reg_y(uint8_t value)
{
    cpu->setY(value);
}

bool cpu_device::is_reg_status_negative_flag()
{
    return cpu->getStatus() & NEGATIVE > 0;
}

void cpu_device::set_reg_status_negative_flag(bool value)
{
    cpu->setStatus(cpu->getStatus() & !NEGATIVE | value * NEGATIVE);
}

bool cpu_device::is_reg_status_overflow_flag()
{
    return cpu->getStatus() & OVERFLOW > 0;
}

void cpu_device::set_reg_status_overflow_flag(bool value)
{
    cpu->setStatus(cpu->getStatus() & !OVERFLOW | value * OVERFLOW);
}

bool cpu_device::is_reg_status_break_flag()
{
    return cpu->getStatus() & BREAK > 0;
}

void cpu_device::set_reg_status_break_flag(bool value)
{
    cpu->setStatus(cpu->getStatus() & !BREAK | value * BREAK);
}

bool cpu_device::is_reg_status_decimal_flag()
{
    return cpu->getStatus() & DECIMAL > 0;
}

void cpu_device::set_reg_status_decimal_flag(bool value)
{
    cpu->setStatus(cpu->getStatus() & !DECIMAL | value * DECIMAL);
}

bool cpu_device::is_reg_status_interrupt_flag()
{
    return cpu->getStatus() & INTERRUPT > 0;
}

void cpu_device::set_status_interrupt_flag(bool value)
{
    cpu->setStatus(cpu->getStatus() & !INTERRUPT | value * INTERRUPT);
}

bool cpu_device::is_reg_status_zero_flag()
{
    return cpu->getStatus() & ZERO > 0;
}

void cpu_device::set_status_zero_flag(bool value)
{
    cpu->setStatus(cpu->getStatus() & !ZERO | value * ZERO);
}

bool cpu_device::is_reg_status_carry_flag()
{
    return cpu->getStatus() & CARRY > 0;
}

void cpu_device::set_status_carry_flag(bool value)
{
    cpu->setStatus(cpu->getStatus() & !CARRY | value * CARRY);
}

void cpu_device::print()
{
    printf("CPU result:\n A: $%X, X: $%X, Y: $%X, S: $%X (N: %s, O: %s, B: %s, D: %s, I: %s, Z: %s, C: %s), PC: $%X\n",
           cpu->getA(),
           cpu->getX(),
           cpu->getY(),
           cpu->getStatus(),
           (cpu->getStatus() & NEGATIVE) > 0 ? "True" : "False",
           (cpu->getStatus() & OVERFLOW) > 0 ? "True" : "False",
           (cpu->getStatus() & BREAK) > 0 ? "True" : "False",
           (cpu->getStatus() & DECIMAL) > 0 ? "True" : "False",
           (cpu->getStatus() & INTERRUPT) > 0 ? "True" : "False",
           (cpu->getStatus() & ZERO) > 0 ? "True" : "False",
           (cpu->getStatus() & CARRY) > 0 ? "True" : "False",
           cpu->getPC());
}