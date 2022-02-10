#include "condition/condition_register_pc.h"
#include "util/address_convert.h"

condition_register_pc::condition_register_pc(emulation_devices *_device, json condition)
{
    address = address_convert::get_address(_device, condition);
}

uint16_t condition_register_pc::get_address()
{
    return address;
}
