#include "condition/condition_memory_value.hpp"
#include "util/value_convert.hpp"
#include "util/expression_execute.hpp"

condition_memory_value::condition_memory_value(emulation_devices *device, uint16_t _address, json condition, bool _permanent, string _name)
{
    address = _address;
    name = _name;
    permanent = _permanent;
    for (json &value_def : condition)
        if (expression_execute::find(value_def))
            expression_sequence.push_back(
                condition_expression<expression_two_complement_byte, uint8_t>(device, value_def));
        else
            sequence.push_back(
                value_convert::to_two_complement_byte(device, value_def));
}

uint16_t condition_memory_value::get_address()
{
    return address;
}

string condition_memory_value::get_name()
{
    return name;
}

bool condition_memory_value::is_permanent()
{
    return permanent;
}

vector<uint8_t> condition_memory_value::get_sequence()
{
    return sequence;
}

vector<condition_expression<expression_two_complement_byte, uint8_t>> condition_memory_value::get_expressions()
{
    return expression_sequence;
}
