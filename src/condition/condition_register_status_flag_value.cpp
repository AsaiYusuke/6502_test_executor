#include "condition/condition_register_status_flag_value.hpp"

#include "util/expression_execute.hpp"
#include "util/value_convert.hpp"

condition_register_status_flag_value::condition_register_status_flag_value(emulation_devices *device, json condition)
{
    expression = new condition_expression<expression_value, bool>(
        device,
        expression_execute::find(condition) ? condition : json::object());
    value = condition.is_boolean() ? condition.get<bool>() : false;
}

bool condition_register_status_flag_value::get_value()
{
    return value;
}

condition_expression<expression_value, bool> *condition_register_status_flag_value::get_expression()
{
    return expression;
}
