#include "test_setup.h"

test_setup::test_setup(condition *cond)
{
    this->cond = cond;
}

bool test_setup::execute()
{
    cond->get_device()->clear(cond->get_pc_register_def()->get_address());

    cpu_device *cpu_dev = cond->get_device()->get_cpu();
    for (auto register_def : cond->get_register_defs())
    {
        if (register_def.is_blank())
            continue;
        cpu_dev->set_register8(register_def.get_type(), register_def.get_value());
    }
    uint8_t status;
    for (auto status_flag_def : cond->get_status_flag_defs())
    {
        status |= ((uint8_t)get<0>(status_flag_def) * get<1>(status_flag_def));
    }
    cpu_dev->set_register8(register_type::Status, status);

    memory_device *mem_dev = cond->get_device()->get_memory();
    for (auto memory_def : cond->get_memory_value_defs())
    {
        mem_dev->set_read_sequence(get<0>(memory_def), get<1>(memory_def));
    }

    return true;
}