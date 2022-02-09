#include "condition/condition.h"

condition::condition(emulation_devices *_device, json condition, json target)
    : condition::condition(_device, condition)
{
    register_pc = new condition_pc_register(_device, target);
}

condition::condition(emulation_devices *_device, json condition_json)
{
    device = _device;

    register_defs.push_back(condition_register(register_type::A, device, condition_json["A"]));
    register_defs.push_back(condition_register(register_type::X, device, condition_json["X"]));
    register_defs.push_back(condition_register(register_type::Y, device, condition_json["Y"]));

    if (!condition_json["Status"].is_null())
        for (auto &element : condition_json["Status"].items())
            status_flag_defs.push_back(
                condition_status_flag(
                    device,
                    element.key(),
                    element.value()));

    if (!condition_json["memory"].is_null())
        for (auto &memory_def : condition_json["memory"])
            memory_defs.push_back(
                condition_memory(
                    device,
                    memory_def));
}

emulation_devices *condition::get_device()
{
    return device;
}

vector<condition_register> condition::get_register_defs()
{
    return register_defs;
}

vector<condition_status_flag> condition::get_status_flag_defs()
{
    return status_flag_defs;
}

vector<condition_memory> condition::get_memory_defs()
{
    return memory_defs;
}

condition_pc_register *condition::get_pc_register_def()
{
    return register_pc;
}
