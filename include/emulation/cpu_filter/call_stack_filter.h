#pragma once

#include <vector>

#include "emulation/cpu_filter/cpu_filter.h"
#include "emulation/cpu_device.h"

class call_stack_filter : public i_cpu_filter
{
private:
    enum class inst_type
    {
        call,
        retern
    };
    cpu_device *cpu;
    bool isCallInstr;
    bool isReternInstr;
    bool isInterruptInstr;
    bool isReturned;
    bool willReturn;
    vector<pair<inst_type, uint16_t>> call_stack;
    uint16_t pre_pc;

public:
    call_stack_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
    bool is_returned_instruction();
    vector<uint16_t> get_call_stack();
};