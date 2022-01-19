#include "condition/condition_register.h"
#include "condition/address_convert.h"

condition_register::condition_register(register_type _type, emulation_devices *device, json condition)
{
    type = _type;
    blank = condition.is_null();
    value = address_convert::to_two_complement_byte(device, condition);
}

register_type condition_register::get_type()
{
    return type;
}

bool condition_register::is_blank()
{
    return blank;
}

uint8_t condition_register::get_value()
{
    return value;
}
