#include "condition/condition_mocked_value.h"

condition_mocked_value::condition_mocked_value(emulation_devices *_device, json condition_json)
    : condition(_device, condition_json)
{
}

vector<condition_register_a_x_y> condition_mocked_value::get_register_defs()
{
    return condition::get_register_defs();
}

vector<condition_register_status_flag> condition_mocked_value::get_status_flag_defs()
{
    return condition::get_status_flag_defs();
}

vector<condition_memory> condition_mocked_value::get_memory_defs()
{
    return condition::get_memory_defs();
}

void condition_mocked_value::execute()
{
}