#include "condition/condition_pc_register.h"

condition_pc_register::condition_pc_register(emulation_devices *_device, json condition)
{
    address = address_convert::get_address(_device, condition);
}

uint16_t condition_pc_register::get_address()
{
    return address;
}
