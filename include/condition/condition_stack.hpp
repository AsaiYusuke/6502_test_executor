#pragma once

#include "nlohmann/json.hpp"

#include "condition/condition_expression.hpp"
#include "condition/expression_two_complement_byte.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

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
