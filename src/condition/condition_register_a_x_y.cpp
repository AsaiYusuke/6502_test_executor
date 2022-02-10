#include "condition/condition_register_a_x_y.h"
#include "condition/address_convert.h"

condition_register_a_x_y::condition_register_a_x_y(emulation_devices *device, string _name, json condition)
{
    name = _name;
    type = register_name_type_map[name];
    value = address_convert::to_two_complement_byte(device, condition);
}

string condition_register_a_x_y::get_name()
{
    return name;
}

register_type condition_register_a_x_y::get_type()
{
    return type;
}

uint8_t condition_register_a_x_y::get_value()
{
    return value;
}