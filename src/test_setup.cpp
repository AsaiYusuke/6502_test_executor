#include "test_setup.h"
#include "emulation/emulation_devices.h"

test_setup::test_setup(emulation_devices *device, json condition_json, json target)
    : condition(device, condition_json, target)
{
}

void test_setup::execute()
{
    get_device()->clear(
        get_register_pc_def()->get_start_address(),
        get_register_pc_def()->get_end_address(),
        get_stack_def()->get_stack());

    cpu_device *cpu_dev = get_device()->get_cpu();
    for (auto register_def : get_register_defs())
        cpu_dev->set_register(register_def.get_type(), register_def.get_value());

    uint8_t status_bits = 0;
    for (auto status_flag_def : get_status_flag_defs())
        status_bits |= ((uint8_t)status_flag_def.get_type() * status_flag_def.get_value());
    cpu_dev->set_register(register_type::P, status_bits);

    for (auto interrupt_def : get_interrupt_defs())
        cpu_dev->add_interrupt_hook(interrupt_def.get_type(), interrupt_def.get_entry_point());

    for (auto mocked_proc_def : get_mocked_proc_defs())
        cpu_dev->add_mocked_proc_hook(mocked_proc_def);

    memory_device *mem_dev = get_device()->get_memory();
    for (auto memory_def : get_memory_defs())
        for (auto memory_value_def : memory_def.get_value_sequences())
            mem_dev->set_read_sequence(memory_value_def.get_address(), memory_value_def.get_sequence(), memory_value_def.is_permanent());
}
