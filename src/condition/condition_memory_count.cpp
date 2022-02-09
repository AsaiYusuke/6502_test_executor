#include "condition/condition_memory_count.h"

condition_memory_count::condition_memory_count(uint16_t _address, uint8_t _count, string _name)
{
    address = _address;
    count = _count;
    name = _name;
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
