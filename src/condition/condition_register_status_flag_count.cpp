#include "condition/condition_register_status_flag_count.h"

condition_register_status_flag_count::condition_register_status_flag_count(emulation_devices *device, json condition)
{
    expression = new condition_expression<expression_value, uint8_t>(
        device,
        expression_executer::find(condition) ? condition : json::object());
}

condition_expression<expression_value, uint8_t> *condition_register_status_flag_count::get_expression()
{
    return expression;
}
