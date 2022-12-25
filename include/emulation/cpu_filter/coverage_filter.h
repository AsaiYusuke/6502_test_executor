#pragma once

#include <vector>

#include "emulation/cpu_filter/cpu_filter.h"

class cpu_device;
class debug_info;

class coverage_filter : public i_cpu_filter
{
private:
    cpu_device *cpu;
    debug_info *debug;

public:
    coverage_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
    void set_debug(debug_info *debug);
};
