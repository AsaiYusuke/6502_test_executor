#include <vector>

#include "emulation/emulation_devices.h"

cpu_device::cpu_device(emulation_devices *_device, args_parser *args, json config)
{
    device = _device;

    cpu = new mos6502((i_memory_access *)_device->get_memory());

    if (config["timeout"].is_null())
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

    call_stack.clear();
    call_stack.push_back(0xFFFF);
    call_stack.push_back(target_program_counter);
}

void cpu_device::execute()
{
    int count = 0;
    int32_t cyclesRemaining;
    uint64_t cycleCount = 0;
    do
    {
        bool isCallInstr = false;

        currentPC = cpu->getPC();

        if (cpu->isCallInstr())
        {
            call_stack.push_back(cpu->getPC());
            isCallInstr = true;
        }
        else if (cpu->isReturnInstr())
        {
            call_stack.pop_back();
            call_stack.pop_back();
        }

        cyclesRemaining = 1;
        cpu->Run(cyclesRemaining, cycleCount, cpu->INST_COUNT);

        if (isCallInstr)
            call_stack.push_back(cpu->getPC());

    } while (cpu->getPC() != 0xFFFF && ++count < get_timeout_threshold());

    if (count >= get_timeout_threshold())
        device->add_error_reuslt(runtime_error_type::TIMEOUT);
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
    printf("CPU result:\n  A: $%X, X: $%X, Y: $%X, P: $%X (N: %s, O: %s, B: %s, D: %s, I: %s, Z: %s, C: %s), PC: $%04X\n",
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
    vector<uint16_t> current_call_stack = call_stack;
    current_call_stack.push_back(currentPC);
    return current_call_stack;
}