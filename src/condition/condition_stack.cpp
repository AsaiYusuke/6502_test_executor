#include "condition/condition_stack.h"
#include "util/value_convert.h"
#include "util/expression_executer.h"

condition_stack::condition_stack(emulation_devices *device, json condition)
{
    if (!condition.is_array())
        return;

    for (json &stack_def : condition)
        if (expression_executer::find(stack_def))
        {
            vector<pair<operator_type, uint8_t>> expressions;
            for (auto &expression : expression_executer::get(stack_def))
                expressions.push_back(
                    make_pair(
                        expression.first,
                        value_convert::to_two_complement_byte(device, expression.second)));
            expression_stack.push_back(expressions);
        }
        else
            stack.push_back(
                value_convert::to_two_complement_byte(device, stack_def));
}

vector<uint8_t> condition_stack::get_stack()
{
    return stack;
}
vector<vector<pair<operator_type, uint8_t>>> condition_stack::get_expressions()
{
    return expression_stack;
}
