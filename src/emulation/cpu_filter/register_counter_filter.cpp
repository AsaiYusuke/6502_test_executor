#include "emulation/cpu_filter/register_counter_filter.h"

register_counter_filter::register_counter_filter(cpu_device *cpu)
{
    this->cpu = cpu;
    register_types.push_back(register_type::A);
    register_types.push_back(register_type::X);
    register_types.push_back(register_type::Y);
    register_types.push_back(register_type::S);
    register_types.push_back(register_type::PC);
    status_types.push_back(status_flag_type::Negative);
    status_types.push_back(status_flag_type::Overflow);
    status_types.push_back(status_flag_type::Constant);
    status_types.push_back(status_flag_type::Break);
    status_types.push_back(status_flag_type::Decimal);
    status_types.push_back(status_flag_type::InterruptDisable);
    status_types.push_back(status_flag_type::Zero);
    status_types.push_back(status_flag_type::Carry);
}


void register_counter_filter::clear()
{
    register_read_counts.clear();
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

uint8_t register_counter_filter::get_read_count(register_type type)
{
    return register_read_counts[type];
}

uint8_t register_counter_filter::get_write_count(register_type type)
{
    return register_write_counts[type];
}

uint8_t register_counter_filter::get_read_count(status_flag_type type)
{
    return status_read_counts[type];
}

uint8_t register_counter_filter::get_write_count(status_flag_type type)
{
    return status_write_counts[type];
}
