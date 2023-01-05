#pragma once

#include "condition/condition_expression.hpp"
#include "condition/expression_two_complement_byte.hpp"

using namespace std;

class condition_register_a_x_y_value
{
private:
    uint8_t value;
    condition_expression<expression_two_complement_byte, uint8_t> *expression;

public:
    condition_register_a_x_y_value(emulation_devices *device, json condition);
    uint8_t get_value();
    condition_expression<expression_two_complement_byte, uint8_t> *get_expression();
};
