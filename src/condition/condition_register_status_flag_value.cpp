#include "condition/condition_register_status_flag_value.hpp"

#include "util/expression_execute.hpp"
#include "util/value_convert.hpp"

using namespace std;

condition_register_status_flag_value::condition_register_status_flag_value(const emulation_devices *device, json condition)
{
    expression = new condition_expression<expression_value, bool>(
        device,
        expression_execute::find(condition) ? condition : json::object());
    value = condition.is_boolean() ? condition.get<bool>() : false;
}

bool condition_register_status_flag_value::get_value() const
{
    return value;
}

condition_expression<expression_value, bool> *condition_register_status_flag_value::get_expression() const
{
    return expression;
}
