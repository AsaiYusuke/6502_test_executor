#pragma once

#include <vector>

#include "emulation/cpu_filter/cpu_filter.hpp"

class cpu_device;
class debug_info;

class coverage_filter : public i_cpu_filter
{
private:
    cpu_device *cpu;
    debug_info *debug;

public:
    explicit coverage_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
    void set_debug(debug_info *debug);
};
