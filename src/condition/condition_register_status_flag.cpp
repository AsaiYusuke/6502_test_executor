#include "condition/condition_register_status_flag.h"
#include "util/expression_executer.h"

condition_register_status_flag::condition_register_status_flag(emulation_devices *device, string _name, json condition)
{
    name = _name;
    type = status_flag_name_type_map[name];
    if (expression_executer::find(condition))
        expression =
            new condition_expression<expression_value, bool>(device, condition);
    else
        value = condition.get<bool>();
}

string condition_register_status_flag::get_name()
{
    return name;
}

status_flag_type condition_register_status_flag::get_type()
{
    return type;
}

bool condition_register_status_flag::get_value()
{
    return value;
}

condition_expression<expression_value, bool> *condition_register_status_flag::get_expression()
{
    return expression;
}
