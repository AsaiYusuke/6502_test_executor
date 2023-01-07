#include "condition/condition_register_status_flag_count.hpp"

condition_register_status_flag_count::condition_register_status_flag_count(const emulation_devices *device, json condition)
{
    expression = new condition_expression<expression_value, uint8_t>(
        device,
        expression_execute::find(condition) ? condition : json::object());
}

condition_expression<expression_value, uint8_t> *condition_register_status_flag_count::get_expression() const
{
    return expression;
}
