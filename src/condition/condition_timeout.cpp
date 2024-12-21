#include "condition/condition_timeout.hpp"

#include "util/expression_execute.hpp"

using namespace std;

condition_timeout::condition_timeout(const emulation_devices *device, json condition)
{
    expression =
        new condition_expression<expression_value, bool>(
            device,
            expression_execute::find(condition) ? condition : json({{operator_type_name_map[operator_type::EQ], false}}));
}

condition_expression<expression_value, bool> *condition_timeout::get_expression() const
{
    return expression;
}
