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
    cpu->SetPC(target_program_counter);
    cpu->StackPush((TEST_RETURN_ADDRESS - 1) >> 8);
    cpu->StackPush((TEST_RETURN_ADDRESS - 1) & 0xFF);

    call_stack.clear();
    call_stack.push_back(TEST_RETURN_ADDRESS);
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

        currentPC = cpu->GetPC();

        if (cpu->isCallInstr())
        {
            call_stack.push_back(cpu->GetPC());
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
            call_stack.push_back(cpu->GetPC());

    } while (cpu->GetPC() != TEST_RETURN_ADDRESS && ++count < get_timeout_threshold());

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
        return cpu->GetA();
    case register_type::X:
        return cpu->GetX();
    case register_type::Y:
        return cpu->GetY();
    case register_type::P:
        return cpu->GetP();
    }
    return 0;
}

void cpu_device::set_register(register_type type, uint8_t value)
{
    switch (type)
    {
    case register_type::A:
        return cpu->SetResetA(value);
    case register_type::X:
        return cpu->SetResetX(value);
    case register_type::Y:
        return cpu->SetResetY(value);
    case register_type::P:
        return cpu->SetResetP(value);
    }
}

void cpu_device::print()
{
    printf("Register result:\n");
    printf("  A   $%X\n", cpu->GetA());
    printf("  X   $%X\n", cpu->GetX());
    printf("  Y   $%X\n", cpu->GetY());
    printf("  P   $%X (N: %s, O: %s, B: %s, D: %s, I: %s, Z: %s, C: %s)\n",
           cpu->GetP(),
           (cpu->GetP() & NEGATIVE) > 0 ? "True" : "False",
           (cpu->GetP() & OVERFLOW) > 0 ? "True" : "False",
           (cpu->GetP() & BREAK) > 0 ? "True" : "False",
           (cpu->GetP() & DECIMAL) > 0 ? "True" : "False",
           (cpu->GetP() & INTERRUPT) > 0 ? "True" : "False",
           (cpu->GetP() & ZERO) > 0 ? "True" : "False",
           (cpu->GetP() & CARRY) > 0 ? "True" : "False");
    printf("  PC  $%04X\n", cpu->GetPC());
}

vector<uint16_t> cpu_device::get_call_stack()
{
    vector<uint16_t> current_call_stack = call_stack;
    current_call_stack.push_back(currentPC);
    return current_call_stack;
}