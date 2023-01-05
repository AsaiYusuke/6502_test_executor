#include "condition/condition_stack.hpp"

#include "util/expression_execute.hpp"
#include "util/value_convert.hpp"

condition_stack::condition_stack(emulation_devices *device, json condition)
{
    if (!condition.is_array())
        return;

    for (json &stack_def : condition)
        if (expression_execute::find(stack_def))
            expression_stack.push_back(
                condition_expression<expression_two_complement_byte, uint8_t>(device, stack_def));
        else
            stack.push_back(
                value_convert::to_two_complement_byte(device, stack_def));
}

vector<uint8_t> condition_stack::get_stack()
{
    return stack;
}

vector<condition_expression<expression_two_complement_byte, uint8_t>> condition_stack::get_expressions()
{
    return expression_stack;
}
