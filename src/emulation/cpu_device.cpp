#include <vector>

#include "emulation/cpu_device.h"

cpu_device::cpu_device(args_parser *args, json config, i_memory_access *memory_access)
{
    cpu = new mos6502(memory_access);
    
    if (config.is_null() || config["timeout"].is_null())
        timeout_threshold = args->get_test_timeout();
    else
        timeout_threshold = config["timeout"].get<int>();
}

void cpu_device::clear(uint16_t target_program_counter)
{
    cpu->Reset();

    cpu->setPC(target_program_counter);
    cpu->StackPush(0xFF);
    cpu->StackPush(0xFE);

    callStack.clear();
    callStack.push_back(0xFFFF);
    callStack.push_back(target_program_counter);

    timeout = false;
}

void cpu_device::execute()
{
    int count = 0;
    int32_t cyclesRemaining;
    uint64_t cycleCount = 0;
    do
    {
        bool isCallInstr = false;

        if (cpu->isCallInstr())
        {
            callStack.push_back(cpu->getPC());
            isCallInstr = true;
        }
        else if (cpu->isReturnInstr())
        {
            callStack.pop_back();
            callStack.pop_back();
        }

        cyclesRemaining = 1;
        cpu->Run(cyclesRemaining, cycleCount, cpu->INST_COUNT);

        if (isCallInstr)
            callStack.push_back(cpu->getPC());

    } while (cpu->getPC() != 0xFFFF && ++count < get_timeout_threshold());

    if (count >= get_timeout_threshold())
        timeout = true;
}

bool cpu_device::is_timeout()
{
    return timeout;
}

int cpu_device::get_timeout_threshold()
{
    return timeout_threshold;
}

uint8_t cpu_device::get_register(register_type type)
{
    switch (type)
    {
    case register_type::A:
        return cpu->getA();
    case register_type::X:
        return cpu->getX();
    case register_type::Y:
        return cpu->getY();
    case register_type::P:
        return cpu->getP();
    }
    return 0;
}

void cpu_device::set_register(register_type type, uint8_t value)
{
    switch (type)
    {
    case register_type::A:
        return cpu->setA(value);
    case register_type::X:
        return cpu->setX(value);
    case register_type::Y:
        return cpu->setY(value);
    case register_type::P:
        return cpu->setP(value);
    }
}

void cpu_device::print()
{
    printf("CPU result:\n  A: $%X, X: $%X, Y: $%X, S: $%X (N: %s, O: %s, B: %s, D: %s, I: %s, Z: %s, C: %s), PC: $%X\n",
           cpu->getA(),
           cpu->getX(),
           cpu->getY(),
           cpu->getP(),
           (cpu->getP() & NEGATIVE) > 0 ? "True" : "False",
           (cpu->getP() & OVERFLOW) > 0 ? "True" : "False",
           (cpu->getP() & BREAK) > 0 ? "True" : "False",
           (cpu->getP() & DECIMAL) > 0 ? "True" : "False",
           (cpu->getP() & INTERRUPT) > 0 ? "True" : "False",
           (cpu->getP() & ZERO) > 0 ? "True" : "False",
           (cpu->getP() & CARRY) > 0 ? "True" : "False",
           cpu->getPC());
}

vector<uint16_t> cpu_device::get_call_stack()
{
    callStack.push_back(cpu->getPC());
    return callStack;
}