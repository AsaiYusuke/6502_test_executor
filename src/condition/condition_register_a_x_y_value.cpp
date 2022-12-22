#include "condition/condition_register_a_x_y_value.h"
#include "util/value_convert.h"
#include "util/expression_executer.h"
#include <iostream>

condition_register_a_x_y_value::condition_register_a_x_y_value(emulation_devices *device, json condition)
{
    expression = new condition_expression<expression_two_complement_byte, uint8_t>(
        device,
        expression_execute::find(condition) ? condition : json::object());
    value = value_convert::to_two_complement_byte(device, condition);
}

uint8_t condition_register_a_x_y_value::get_value()
{
    return value;
}

condition_expression<expression_two_complement_byte, uint8_t> *condition_register_a_x_y_value::get_expression()
{
    return expression;
}
