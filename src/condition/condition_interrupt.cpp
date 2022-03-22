#include "condition/condition_interrupt.h"
#include "util/value_convert.h"

condition_interrupt::condition_interrupt(emulation_devices *device, json condition)
{
    type = interrupt_name_type_map[condition["type"]];
    hook_address = value_convert::get_address(device, condition["hook"]);
}

interrupt_type condition_interrupt::get_type()
{
    return type;
}

uint8_t condition_interrupt::get_hook_address()
{
    return hook_address;
}
