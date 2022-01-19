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

void cpu_device::execute()
{
    int count = 0;
    uint64_t cycleCount = 0;
    do
    {
        cpu->Run(1, cycleCount, cpu->INST_COUNT);
    } while (cpu->getPC() != 0xFFFF && ++count < timeout());

    if (count >= timeout())
    {
        cerr << "Execution count >= " + to_string(timeout()) << endl;
        throw 2;
    }
}

int cpu_device::timeout()
{
    return DEFAULT_TIMEOUT;
}

uint16_t cpu_device::get_register16(register_type type)
{
    switch (type)
    {
    case register_type::PC:
        return cpu->getPC();
    }
    cerr << "invalid register16: '" << register_type_name_map[type] << endl;
    throw 2;
}

void cpu_device::set_register16(register_type type, uint16_t value)
{
    switch (type)
    {
    case register_type::PC:
        cpu->setPC(value);
    }
    cerr << "invalid register16: '" << register_type_name_map[type] << endl;
    throw 2;
}

uint8_t cpu_device::get_register8(register_type type)
{
    switch (type)
    {
    case register_type::A:
        return cpu->getA();
    case register_type::X:
        return cpu->getX();
    case register_type::Y:
        return cpu->getY();
    case register_type::Status:
        return cpu->getStatus();
    }
    cerr << "invalid register8: '" << register_type_name_map[type] << endl;
    throw 2;
}

void cpu_device::set_register8(register_type type, uint8_t value)
{
    switch (type)
    {
    case register_type::A:
        return cpu->setA(value);
    case register_type::X:
        return cpu->setX(value);
    case register_type::Y:
        return cpu->setY(value);
    case register_type::Status:
        return cpu->setStatus(value);
    }
    cerr << "invalid register8: '" << register_type_name_map[type] << endl;
    throw 2;
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