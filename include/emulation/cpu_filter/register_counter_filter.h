#pragma once

#include <vector>

#include "emulation/cpu_filter/cpu_filter.h"
#include "enum/register_type.h"
#include "enum/status_flag_type.h"

class cpu_device;

class register_counter_filter : public i_cpu_filter
{
private:
    cpu_device *cpu;
    vector<register_type> register_types;
    vector<status_flag_type> status_types;
    map<register_type, uint8_t> register_read_counts;
    map<register_type, uint8_t> register_write_counts;
    map<status_flag_type, uint8_t> status_read_counts;
    map<status_flag_type, uint8_t> status_write_counts;

public:
    register_counter_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
    uint8_t get_read_count(register_type type);
    uint8_t get_write_count(register_type type);
    uint8_t get_read_count(status_flag_type type);
    uint8_t get_write_count(status_flag_type type);
};
