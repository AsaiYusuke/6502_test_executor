#include "condition/condition_register_pc.h"
#include "util/value_convert.h"

condition_register_pc::condition_register_pc(emulation_devices *_device, json condition)
{
    start_address = value_convert::get_address(_device, condition["start"]);
    if (condition["end"].is_null())
        end_address = TEST_RETURN_ADDRESS;
    else
        end_address = value_convert::get_address(_device, condition["end"]);
}

uint16_t condition_register_pc::get_start_address()
{
    return start_address;
}

uint16_t condition_register_pc::get_end_address()
{
    return end_address;
}
