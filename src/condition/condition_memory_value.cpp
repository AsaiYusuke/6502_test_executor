#include "condition/condition_memory_value.h"

condition_memory_value::condition_memory_value(uint16_t _address, vector<uint8_t> _sequence, bool _permanent, string _name)
{
    address = _address;
    sequence = _sequence;
    permanent = _permanent;
    name = _name;
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
