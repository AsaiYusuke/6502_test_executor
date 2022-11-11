#include "condition/condition_memory_count.h"
#include "condition/condition_expression.h"
#include "condition/expression_value.h"
#include "util/expression_executer.h"

condition_memory_count::condition_memory_count(emulation_devices *device, uint16_t _address, json condition, string _name)
{
    address = _address;
    name = _name;
    expression = new condition_expression<expression_value, uint8_t>(device, condition);
}

uint16_t condition_memory_count::get_address()
{
    return address;
}

string condition_memory_count::get_name()
{
    return name;
}

condition_expression<expression_value, uint8_t> *condition_memory_count::get_expression()
{
    return expression;
}
