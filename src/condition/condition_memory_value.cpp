#include "condition/condition_memory_value.h"
#include "util/value_convert.h"
#include "util/expression_executer.h"

condition_memory_value::condition_memory_value(emulation_devices *device, uint16_t _address, json condition, bool _permanent, string _name)
{
    name = _name;
    address = _address;
    permanent = _permanent;
    for (json &value_def : condition)
        if (expression_executer::find(value_def))
        {
            vector<pair<operator_type, uint8_t>> expressions;
            for (auto &expression : expression_executer::get(value_def))
                expressions.push_back(
                    make_pair(
                        expression.first,
                        value_convert::to_two_complement_byte(device, expression.second)));
            expression_sequence.push_back(expressions);
        }
        else
            sequence.push_back(
                value_convert::to_two_complement_byte(device, value_def));
}

bool condition_memory_value::is_permanent()
{
    return permanent;
}

uint16_t condition_memory_value::get_address()
{
    return address;
}

vector<uint8_t> condition_memory_value::get_sequence()
{
    return sequence;
}

string condition_memory_value::get_name()
{
    return name;
}

vector<vector<pair<operator_type, uint8_t>>> condition_memory_value::get_expressions()
{
    return expression_sequence;
}
