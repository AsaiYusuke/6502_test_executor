#include "emulation/cpu_filter/call_stack_filter.hpp"

#include "emulation/cpu_device.hpp"
#include "util/constant.hpp"

call_stack_filter::call_stack_filter(cpu_device *_cpu)
{
    cpu = _cpu;
}

void call_stack_filter::clear()
{
    call_stack.clear();
    call_stack.push_back(make_pair(inst_type::call, DEFAULT_TEST_RETURN_ADDRESS));
    call_stack.push_back(make_pair(inst_type::call, cpu->get_register16(register_type::PC)));
}

bool call_stack_filter::pre()
{
    isCallInstr = false;
    isReturnInstr = false;
    isInterruptInstr = false;

    pre_pc = cpu->get_register16(register_type::PC);

    if (cpu->is_call_instruction())
    {
        call_stack.push_back(make_pair(inst_type::call, pre_pc));
        isCallInstr = true;
    }
    else if (cpu->is_return_instruction())
    {
        if (call_stack.back().first == inst_type::call)
        {
            call_stack.pop_back();
            call_stack.pop_back();
        }
        else
        {
            call_stack.push_back(make_pair(inst_type::return_back, pre_pc));
            isReturnInstr = true;
        }
    }

    if (!isReturned && cpu->is_interrupt_instruction())
    {
        call_stack.push_back(make_pair(inst_type::call, pre_pc));
        isInterruptInstr = true;
    }

    willReturn = cpu->is_return_instruction() && call_stack.back().first == inst_type::call;

    return true;
}

bool call_stack_filter::post()
{
    if (isCallInstr)
    {
        call_stack.push_back(make_pair(inst_type::call, cpu->get_register16(register_type::PC)));
    }
    if (isReturnInstr)
    {
        call_stack.push_back(make_pair(inst_type::return_back, cpu->get_register16(register_type::PC)));
    }
    if (!isReturned && isInterruptInstr)
    {
        call_stack.push_back(make_pair(inst_type::call, cpu->get_register16(register_type::PC)));
    }

    isReturned = willReturn;

    return true;
}

bool call_stack_filter::is_returned_instruction() const
{
    return isReturned;
}

vector<uint16_t> call_stack_filter::get_call_stack() const
{
    vector<uint16_t> current_call_stack;
    for (auto entry : call_stack)
        current_call_stack.push_back(entry.second);
    current_call_stack.push_back(pre_pc);
    return current_call_stack;
}