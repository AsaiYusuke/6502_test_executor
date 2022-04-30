#include "condition/condition_interrupt.h"
#include "util/value_convert.h"

condition_interrupt::condition_interrupt(emulation_devices *device, json condition)
{
    type = interrupt_name_type_map[condition["type"]];
    entryPoint = value_convert::get_address(device, condition["entryPoint"]);
}

interrupt_type condition_interrupt::get_type()
{
    return type;
}

uint16_t condition_interrupt::get_entry_point()
{
    return entryPoint;
}
