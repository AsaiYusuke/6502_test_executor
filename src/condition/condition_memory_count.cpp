#include <cstdint>

#include "condition/condition_memory_count.hpp"

#include "condition/condition_expression.hpp"
#include "condition/expression_value.hpp"
#include "util/expression_execute.hpp"

condition_memory_count::condition_memory_count(const emulation_devices *device, uint16_t _address, json condition, const string &_name)
{
    address = _address;
    name = _name;
    expression = new condition_expression<expression_value, uint8_t>(device, condition);
}

uint16_t condition_memory_count::get_address() const
{
    return address;
}

string condition_memory_count::get_name() const
{
    return name;
}

condition_expression<expression_value, uint8_t> *condition_memory_count::get_expression() const
{
    return expression;
}
