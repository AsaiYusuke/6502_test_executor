#include "emulation/cpu_filter/call_stack_filter.h"
#include "util/constant.h"

call_stack_filter::call_stack_filter(cpu_device *cpu)
{
    this->cpu = cpu;
}

void call_stack_filter::clear()
{
    call_stack.clear();
    call_stack.push_back(make_pair(inst_type::call, TEST_RETURN_ADDRESS));
    call_stack.push_back(make_pair(inst_type::call, cpu->get_register16(register_type::PC)));
}

bool call_stack_filter::pre()
{
    isCallInstr = false;
    isReternInstr = false;
    isInterruptInstr = false;

    if (cpu->is_call_instrunction())
    {
        call_stack.push_back(make_pair(inst_type::call, cpu->get_register16(register_type::PC)));
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
            call_stack.push_back(make_pair(inst_type::retern, cpu->get_register16(register_type::PC)));
            isReternInstr = true;
        }
    }

    if (!isReturned && cpu->is_interrupt_instruction())
    {
        call_stack.push_back(make_pair(inst_type::call, cpu->get_register16(register_type::PC)));
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
    if (isReternInstr)
    {
        call_stack.push_back(make_pair(inst_type::retern, cpu->get_register16(register_type::PC)));
    }
    if (!isReturned && isInterruptInstr)
    {
        call_stack.push_back(make_pair(inst_type::call, cpu->get_register16(register_type::PC)));
    }

    isReturned = willReturn;

    return true;
}

bool call_stack_filter::is_returned_instruction()
{
    return isReturned;
}

vector<uint16_t> call_stack_filter::get_call_stack()
{
    vector<uint16_t> current_call_stack;
    for (auto entry : call_stack)
        current_call_stack.push_back(entry.second);
    current_call_stack.push_back(cpu->get_register16(register_type::PC));
    return current_call_stack;
}