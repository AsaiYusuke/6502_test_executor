#include "condition/condition_timeout.hpp"

#include "util/expression_execute.hpp"

condition_timeout::condition_timeout(emulation_devices *device, json condition)
{
    if (!expression_execute::find(condition))
        condition = {{operator_type_name_map[operator_type::EQ], false}};

    expression =
        new condition_expression<expression_value, bool>(device, condition);
}

condition_expression<expression_value, bool> *condition_timeout::get_expression()
{
    return expression;
}
