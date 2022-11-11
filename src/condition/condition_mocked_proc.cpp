#include "util/value_convert.h"
#include "condition/condition_mocked_value.h"
#include "condition/condition_mocked_proc.h"

condition_mocked_proc::condition_mocked_proc(emulation_devices *device, json condition)
{
    entry_point = value_convert::get_address(device, condition["entryPoint"]);
    action = mock_action_name_type_map[condition["action"]];
    jmp_dest = value_convert::get_address(device, condition["jmpDest"]);

    if (condition["setValue"].is_array())
        for (auto &mock_value_def : condition["setValue"])
            mock_value_defs.push_back(
                condition_mocked_value(
                    device,
                    mock_value_def));
}

uint16_t condition_mocked_proc::get_entry_point()
{
    return entry_point;
}

mock_action_type condition_mocked_proc::get_action()
{
    return action;
}

uint16_t condition_mocked_proc::get_jmp_dest()
{
    return jmp_dest;
}

condition_mocked_value condition_mocked_proc::get_erased_front_mock_value_def()
{
    auto value =  mock_value_defs.front();
    if (mock_value_defs.size() > 1)
        mock_value_defs.erase(cbegin(mock_value_defs));
    return value;
}