#pragma once

#include <vector>

#include "emulation/cpu_filter/cpu_filter.hpp"
#include "enum/status_flag_type.hpp"
#include "enum/register_type.hpp"

class cpu_device;

class register_counter_filter : public i_cpu_filter
{
private:
    cpu_device *cpu;
    vector<register_type> register_types = {
        register_type::A,
        register_type::X,
        register_type::Y,
        register_type::S,
        register_type::PC
    };
    vector<status_flag_type> status_types = {
        status_flag_type::Negative,
        status_flag_type::Overflow,
        status_flag_type::Constant,
        status_flag_type::Break,
        status_flag_type::Decimal,
        status_flag_type::InterruptDisable,
        status_flag_type::Zero,
        status_flag_type::Carry
    };
    map<register_type, uint8_t> register_read_counts;
    map<register_type, uint8_t> register_write_counts;
    map<status_flag_type, uint8_t> status_read_counts;
    map<status_flag_type, uint8_t> status_write_counts;

public:
    explicit register_counter_filter(cpu_device *cpu);
    void clear();
    bool pre();
    bool post();
    uint8_t get_read_count(register_type type) const;
    uint8_t get_write_count(register_type type) const;
    uint8_t get_read_count(status_flag_type type) const;
    uint8_t get_write_count(status_flag_type type) const;
};
