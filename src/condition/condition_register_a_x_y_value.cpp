#include "condition/condition_register_a_x_y_value.hpp"

#include <iostream>

#include "util/expression_execute.hpp"
#include "util/value_convert.hpp"

using namespace std;

condition_register_a_x_y_value::condition_register_a_x_y_value(const emulation_devices *device, json condition)
{
    expression = new condition_expression<expression_two_complement_byte, uint8_t>(
        device,
        expression_execute::find(condition) ? condition : json::object());
    value = value_convert::to_two_complement_byte(device, condition);
}

uint8_t condition_register_a_x_y_value::get_value() const
{
    return value;
}

condition_expression<expression_two_complement_byte, uint8_t> *condition_register_a_x_y_value::get_expression() const
{
    return expression;
}
