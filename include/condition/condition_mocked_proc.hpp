#pragma once

#include "nlohmann/json.hpp"

#include "enum/mock_action_type.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;
class condition_mocked_value;

class condition_mocked_proc
{
private:
    uint16_t entry_point;
    mock_action_type action;
    uint16_t jmp_dest;
    vector<condition_mocked_value> mock_value_defs;

public:
    condition_mocked_proc(emulation_devices *device, json condition);
    uint16_t get_entry_point() const;
    mock_action_type get_action() const;
    uint16_t get_jmp_dest() const;
    condition_mocked_value get_erased_front_mock_value_def();
};
