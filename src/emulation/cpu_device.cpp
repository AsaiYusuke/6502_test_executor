#include <vector>
#include <typeinfo>

#include "emulation/emulation_devices.h"
#include "enum/cycle_type.h"
#include "util/constant.h"
#include "emulation/cpu_filter/call_stack_filter.h"
#include "emulation/cpu_filter/timeout_check_filter.h"

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

    call_stack = new call_stack_filter(this);
    filters.push_back(call_stack);
    filters.push_back(new timeout_check_filter(this));
}

void cpu_device::clear(uint16_t startPC, uint16_t _endPC, vector<uint8_t> stack)
{
    cpu->Reset();
    cpu->SetPC(startPC);
    cpu->StackPush((TEST_RETURN_ADDRESS - 1) >> 8);
    cpu->StackPush((TEST_RETURN_ADDRESS - 1) & 0xFF);

    for (auto value : stack)
        cpu->StackPush(value);

    endPC = _endPC;

    for (auto filter : filters)
        filter->clear();
}

void cpu_device::execute()
{
    cycle_count = 0;
    do
    {
        for (auto filter : filters)
            if (!filter->pre())
                return;

        if (!is_previous_returned_instruction() && is_interrupt_instruction())
            execute_interrupt();
        else
            if (is_mocked_proc_instruction())
                execute_mocked_proc();
            else
                execute_standard_instruction();

        for (auto filter : filters)
            if (!filter->post())
                return;

    } while (cpu->GetPC() != endPC);
}

void cpu_device::add_error_result(runtime_error_type type)
{
    device->add_error_result(type);
}

uint64_t cpu_device::get_max_cycle_count()
{
    return max_cycle_count;
}

uint64_t cpu_device::get_cycle_count()
{
    return cycle_count;
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
        return cpu->SetA(value);
    case register_type::X:
        return cpu->SetX(value);
    case register_type::Y:
        return cpu->SetY(value);
    case register_type::P:
        return cpu->SetP(value);
    }
}

bool cpu_device::is_call_instrunction()
{
    return cpu->isCallInstr();
}

bool cpu_device::is_return_instruction()
{
    auto it_mocked_proc = mocked_proc_defs.find(cpu->GetPC());
    if (it_mocked_proc != mocked_proc_defs.end())
    {
        auto mocked_proc_def = &it_mocked_proc->second;
        return mocked_proc_def->get_action() == mock_action_type::RTS;
    }

    return cpu->isReturnInstr();
}

bool cpu_device::is_previous_returned_instruction()
{
    return call_stack->is_returned_instruction();
}

bool cpu_device::is_interrupt_instruction()
{
    auto it = interrupt_defs.find(cpu->GetPC());
    return it != interrupt_defs.end();
}

bool cpu_device::is_mocked_proc_instruction()
{
    auto it_mocked_proc = mocked_proc_defs.find(cpu->GetPC());
    return it_mocked_proc != mocked_proc_defs.end();
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

void cpu_device::add_interrupt_hook(interrupt_type type, uint16_t address)
{
    interrupt_defs.insert(make_pair(address, type));
}

void cpu_device::add_mocked_proc_hook(condition_mocked_proc mocked_proc_def)
{
    mocked_proc_defs.insert(
        make_pair(mocked_proc_def.get_entry_point(), mocked_proc_def));
}

void cpu_device::execute_interrupt()
{
    switch (interrupt_defs.find(cpu->GetPC())->second)
    {
    case interrupt_type::NMI:
        cpu->NMI();
        break;
    case interrupt_type::IRQ:
        cpu->IRQ();
        break;
    }
}

void cpu_device::execute_mocked_proc()
{
    auto it_mocked_proc = mocked_proc_defs.find(cpu->GetPC());
    auto mocked_proc_def = &it_mocked_proc->second;
    auto mocked_value_def = mocked_proc_def->get_erased_front_mock_value_def();

    for (auto register_def : mocked_value_def.get_register_defs())
        set_register(register_def.get_type(), register_def.get_value());

    uint8_t status_bits = 0;
    for (auto status_flag_def : mocked_value_def.get_status_flag_defs())
        status_bits |= ((uint8_t)status_flag_def.get_type() * status_flag_def.get_value());
    set_register(register_type::P, status_bits);

    memory_device *mem_dev = device->get_memory();
    for (auto memory_def : mocked_value_def.get_memory_defs())
        for (auto memory_value_def : memory_def.get_value_sequences())
            mem_dev->write_raw(memory_value_def.get_address(), memory_value_def.get_sequence().front());

    switch (mocked_proc_def->get_action())
    {
    case mock_action_type::RTS:
        cpu->forceRts();
        break;
    case mock_action_type::JMP:
        cpu->forceJmp(mocked_proc_def->get_jmp_dest());
        break;
    }
}

void cpu_device::execute_standard_instruction()
{
    cpu->Run(1, cycle_count, cpu->INST_COUNT);
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
    return call_stack->get_call_stack();
}
