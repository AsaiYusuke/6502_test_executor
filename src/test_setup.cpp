#include "test_setup.h"

test_setup::test_setup(emulation_devices *device, json condition_json, json target)
    : condition(device, condition_json, target)
{
}

void test_setup::execute()
{
    get_device()->clear(get_pc_register_def()->get_address());

    cpu_device *cpu_dev = get_device()->get_cpu();
    for (auto register_def : get_register_defs())
    {
        if (register_def.is_blank())
            continue;
        cpu_dev->set_register(register_def.get_type(), register_def.get_value());
    }
    uint8_t status;
    for (auto status_flag_def : get_status_flag_defs())
    {
        status |= ((uint8_t)get<0>(status_flag_def) * get<1>(status_flag_def));
    }
    cpu_dev->set_register(register_type::Status, status);

    memory_device *mem_dev = get_device()->get_memory();
    for (auto memory_def : get_memory_value_defs())
    {
        mem_dev->set_read_sequence(get<0>(memory_def), get<1>(memory_def));
    }
}
