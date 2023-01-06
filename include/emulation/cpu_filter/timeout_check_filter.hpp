#pragma once

#include <vector>

#include "emulation/cpu_filter/cpu_filter.hpp"

class cpu_device;

class timeout_check_filter : public i_cpu_filter
{
private:
    cpu_device *cpu;

public:
    explicit timeout_check_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
};
