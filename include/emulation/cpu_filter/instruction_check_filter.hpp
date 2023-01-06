#pragma once

#include <cstdint>
#include <vector>

#include "emulation/cpu_filter/cpu_filter.hpp"

class cpu_device;

class instruction_check_filter : public i_cpu_filter
{
private:
    cpu_device *cpu;
    uint16_t pre_pc;

public:
    explicit instruction_check_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
};
