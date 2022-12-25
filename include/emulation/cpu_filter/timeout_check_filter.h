#pragma once

#include <vector>

#include "emulation/cpu_filter/cpu_filter.h"

class cpu_device;

class timeout_check_filter : public i_cpu_filter
{
private:
    cpu_device *cpu;

public:
    timeout_check_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
};
