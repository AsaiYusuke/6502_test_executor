#include <vector>

#include "emulation/emulation_devices.h"
#include "enum/cycle_type.h"

#define STACK_ADDRESS(addr) (0x0100 + addr)

cpu_device::cpu_device(emulation_devices *_device, args_parser *args, json config)
{
    device = _device;

    cpu = new mos6502((i_memory_access *)_device->get_memory());

    if (config["maxCycleCount"].is_number())
        max_cycle_count = config["maxCycleCount"].get<uint64_t>();
    else if (config["maxCycleCount"].is_string())
        max_cycle_count =
            cycle_type_value_map[cycle_name_type_map[config["maxCycleCount"].get<string>()]];
    else
        max_cycle_count = args->get_test_timeout();
}

void cpu_device::clear(uint16_t startPC, uint16_t _endPC, vector<uint8_t> stack)
{
    cpu->Reset();
    cpu->SetPC(startPC);
    cpu->StackPush((TEST_RETURN_ADDRESS - 1) >> 8);
    cpu->StackPush((TEST_RETURN_ADDRESS - 1) & 0xFF);

    call_stack.clear();
    call_stack.push_back(make_pair(inst_type::call, TEST_RETURN_ADDRESS));

    for (auto value : stack)
        cpu->StackPush(value);

    call_stack.push_back(make_pair(inst_type::call, startPC));

    endPC = _endPC;
}

void cpu_device::execute()
{
    int32_t cyclesRemaining;
    uint64_t cycle_count = 0;
    do
    {
        bool isCallInstr = false;
        bool isReternInstr = false;

        currentPC = cpu->GetPC();

        if (cpu->isCallInstr())
        {
            call_stack.push_back(make_pair(inst_type::call, cpu->GetPC()));
            isCallInstr = true;
        }
        else if (cpu->isReturnInstr())
        {
            if (call_stack.back().first == inst_type::call)
            {
                call_stack.pop_back();
                call_stack.pop_back();
            }
            else
            {
                call_stack.push_back(make_pair(inst_type::retern, cpu->GetPC()));
                isReternInstr = true;
            }
        }

        cyclesRemaining = 1;
        cpu->Run(cyclesRemaining, cycle_count, cpu->INST_COUNT);

        if (isCallInstr)
            call_stack.push_back(make_pair(inst_type::call, cpu->GetPC()));
        if (isReternInstr)
            call_stack.push_back(make_pair(inst_type::retern, cpu->GetPC()));

    } while (cpu->GetPC() != TEST_RETURN_ADDRESS && cpu->GetPC() != endPC && cycle_count <= get_max_cycle_count());

    if (cycle_count > get_max_cycle_count())
        device->add_error_reuslt(runtime_error_type::TIMEOUT);
}

uint64_t cpu_device::get_max_cycle_count()
{
    return max_cycle_count;
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

vector<uint8_t> cpu_device::get_stack()
{
    vector<uint8_t> result_stack;
    auto resetS = cpu->GetResetS();
    auto curS = cpu->GetS();
    for (auto address = resetS - 2; address > curS; address--)
        result_stack.push_back(device->get_memory()->read_raw(STACK_ADDRESS(address)));
    return result_stack;
}

void cpu_device::print()
{
    printf("Register result:\n");
    printf("  A   $%02X\n", cpu->GetA());
    printf("  X   $%02X\n", cpu->GetX());
    printf("  Y   $%02X\n", cpu->GetY());
    printf("  P   $%02X (N: %s, O: %s, B: %s, D: %s, I: %s, Z: %s, C: %s)\n",
           cpu->GetP(),
           (cpu->GetP() & NEGATIVE) > 0 ? "True" : "False",
           (cpu->GetP() & OVERFLOW) > 0 ? "True" : "False",
           (cpu->GetP() & BREAK) > 0 ? "True" : "False",
           (cpu->GetP() & DECIMAL) > 0 ? "True" : "False",
           (cpu->GetP() & INTERRUPT) > 0 ? "True" : "False",
           (cpu->GetP() & ZERO) > 0 ? "True" : "False",
           (cpu->GetP() & CARRY) > 0 ? "True" : "False");
    printf("  PC  $%04X\n", cpu->GetPC());
    printf("  S   $%02X\n", cpu->GetS());
}

vector<uint16_t> cpu_device::get_call_stack()
{
    vector<uint16_t> current_call_stack;
    for (auto entry : call_stack)
        current_call_stack.push_back(entry.second);
    current_call_stack.push_back(currentPC);
    return current_call_stack;
}