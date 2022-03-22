#include "condition/condition_memory_count.h"
#include "util/expression_executer.h"
#include "condition/condition_expression.h"
#include "condition/expression_value.h"

condition_memory_count::condition_memory_count(emulation_devices *device, uint16_t _address, json condition, string _name)
{
    name = _name;
    address = _address;
    if (expression_executer::find(condition))
        expression =
            new condition_expression<expression_value, uint8_t>(device, condition);
    else
        count = condition.get<uint8_t>();
}

uint16_t condition_memory_count::get_address()
{
    return address;
}

uint8_t condition_memory_count::get_count()
{
    return count;
}

string condition_memory_count::get_name()
{
    return name;
}

condition_expression<expression_value, uint8_t> *condition_memory_count::get_expression()
{
    return expression;
}
