#pragma once

#include "condition/condition.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition_mocked_value : condition
{
public:
    condition_mocked_value(emulation_devices *device, json condition_json);
    vector<condition_register_a_x_y> get_register_defs();
    vector<condition_register_status_flag> get_status_flag_defs();
    vector<condition_memory> get_memory_defs();
    void execute();
};
