#include "condition/condition_memory_count.h"
#include "util/expression_executer.h"

condition_memory_count::condition_memory_count(uint16_t _address, json condition, string _name)
{
    name = _name;
    address = _address;
    if (expression_executer::find(condition))
        for (auto &expression : expression_executer::get(condition))
            expressions.push_back(
                make_pair(
                    expression.first,
                    expression.second.get<uint8_t>()));
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

vector<pair<operator_type, uint8_t>> condition_memory_count::get_expressions()
{
    return expressions;
}
