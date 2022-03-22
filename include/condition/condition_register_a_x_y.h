#pragma once

#include "enum/register_type.h"
#include "condition/condition_expression.h"
#include "condition/expression_two_complement_byte.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_register_a_x_y
{
private:
    string name;
    register_type type;
    uint8_t value;
    condition_expression<expression_two_complement_byte, uint8_t> *expression;

public:
    condition_register_a_x_y(emulation_devices *_device, string _name, json condition);
    string get_name();
    register_type get_type();
    uint8_t get_value();
    condition_expression<expression_two_complement_byte, uint8_t> *get_expression();
};
