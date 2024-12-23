#include "emulation/cpu_filter/register_counter_filter.hpp"

#include "emulation/cpu_device.hpp"

register_counter_filter::register_counter_filter(cpu_device *_cpu)
{
    cpu = _cpu;
}


void register_counter_filter::clear()
{
    for (auto type : register_types)
    {
        register_read_counts[type] = 0;
        register_write_counts[type] = 0;
    }

    for (auto type : status_types)
    {
        status_read_counts[type] = 0;
        status_write_counts[type] = 0;
    }
}

bool register_counter_filter::pre()
{
    for (auto type : register_types)
    {
        if (cpu->is_read_register_instruction(type))
            register_read_counts[type]++;
        if (cpu->is_write_register_instruction(type))
            register_write_counts[type]++;
    }

    for (auto type : status_types)
    {
        if (cpu->is_read_status_instruction(type))
            status_read_counts[type]++;
        if (cpu->is_write_status_instruction(type))
            status_write_counts[type]++;
    }

    return true;
}

bool register_counter_filter::post()
{
    return true;
}

uint8_t register_counter_filter::get_read_count(const register_type type) const
{
    return register_read_counts.at(type);
}

uint8_t register_counter_filter::get_write_count(const register_type type) const
{
    return register_write_counts.at(type);
}

uint8_t register_counter_filter::get_read_count(const status_flag_type type) const
{
    return status_read_counts.at(type);
}

uint8_t register_counter_filter::get_write_count(const status_flag_type type) const
{
    return status_write_counts.at(type);
}
