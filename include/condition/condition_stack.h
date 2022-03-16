#pragma once

#include "enum/operator_type.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_stack
{
private:
    vector<uint8_t> stack;
    vector<vector<pair<operator_type, uint8_t>>> expression_stack;

public:
    condition_stack(emulation_devices *device, json condition);
    vector<uint8_t> get_stack();
    vector<vector<pair<operator_type, uint8_t>>> get_expressions();
};
