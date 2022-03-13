#include "condition/condition_register_status_flag.h"
#include "util/expression_executer.h"

condition_register_status_flag::condition_register_status_flag(emulation_devices *_device, string _name, json condition)
{
    name = _name;
    type = status_flag_name_type_map[name];
    if (expression_executer::find(condition))
        for (auto &expression : expression_executer::get(condition))
            expressions.push_back(
                make_pair(
                    expression.first,
                    expression.second.get<bool>()));
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

vector<pair<operator_type, bool>> condition_register_status_flag::get_expressions()
{
    return expressions;
}
