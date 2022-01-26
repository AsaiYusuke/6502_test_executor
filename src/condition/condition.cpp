#include "condition/condition.h"

condition::condition(emulation_devices *_device, json condition, json target)
    : condition::condition(_device, condition)
{
    register_pc = new condition_pc_register(_device, target);
}

condition::condition(emulation_devices *_device, json condition)
{
    device = _device;

    registers.push_back(condition_register(register_type::A, device, condition["A"]));
    registers.push_back(condition_register(register_type::X, device, condition["X"]));
    registers.push_back(condition_register(register_type::Y, device, condition["Y"]));

    register_status = new condition_status_register(condition["Status"]);

    memory = new condition_memory(device, condition["memory"]);
}

emulation_devices *condition::get_device()
{
    return device;
}

vector<condition_register> condition::get_register_defs()
{
    return registers;
}

vector<tuple<status_flag_type, bool, string>> condition::get_status_flag_defs()
{
    return register_status->get_flags();
}

vector<tuple<uint16_t, vector<uint8_t>, string>> condition::get_memory_value_defs()
{
    return memory->get_memory_value_defs();
}

vector<tuple<uint16_t, uint8_t, string>> condition::get_memory_read_count_defs()
{
    return memory->get_memory_read_count_defs();
}

vector<tuple<uint16_t, uint8_t, string>> condition::get_memory_write_count_defs()
{
    return memory->get_memory_write_count_defs();
}

condition_pc_register *condition::get_pc_register_def()
{
    return register_pc;
}
