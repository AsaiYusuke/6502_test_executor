#include "condition/condition.h"

condition::condition(emulation_devices *_device, json condition, json target)
    : condition::condition(_device, condition)
{
    register_pc_def = new condition_register_pc(_device, target);
}

condition::condition(emulation_devices *_device, json condition_json)
{
    device = _device;

    if (condition_json["register"].is_object())
        for (auto &register_element : condition_json["register"].items())
            switch (register_name_type_map[register_element.key()])
            {
            case register_type::P:
                for (auto &status_element : register_element.value().items())
                    status_flag_defs.push_back(
                        condition_register_status_flag(
                            device,
                            status_element.key(),
                            status_element.value()));
                break;
            default:
                register_defs.push_back(
                    condition_register_a_x_y(
                        device,
                        register_element.key(),
                        register_element.value()));
                break;
            }

    if (condition_json["memory"].is_array())
        for (auto &memory_def : condition_json["memory"])
            memory_defs.push_back(
                condition_memory(
                    device,
                    memory_def));

    if (condition_json["timeout"].is_boolean())
        timeout_def = condition_json["timeout"].get<bool>();
    else
        timeout_def = false;
}

emulation_devices *condition::get_device()
{
    return device;
}

vector<condition_register_a_x_y> condition::get_register_defs()
{
    return register_defs;
}

vector<condition_register_status_flag> condition::get_status_flag_defs()
{
    return status_flag_defs;
}

vector<condition_memory> condition::get_memory_defs()
{
    return memory_defs;
}

condition_register_pc *condition::get_register_pc_def()
{
    return register_pc_def;
}

bool condition::get_timeout_def()
{
    return timeout_def;
}
