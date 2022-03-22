#pragma once

#include "condition/condition_expression.h"
#include "condition/expression_two_complement_byte.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_stack
{
private:
    vector<uint8_t> stack;
    vector<condition_expression<expression_two_complement_byte, uint8_t>> expression_stack;

public:
    condition_stack(emulation_devices *device, json condition);
    vector<uint8_t> get_stack();
    vector<condition_expression<expression_two_complement_byte, uint8_t>> get_expressions();
};
