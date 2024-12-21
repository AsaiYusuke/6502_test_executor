#include <cstdint>

#include "condition/condition_interrupt.hpp"

#include "util/value_convert.hpp"

condition_interrupt::condition_interrupt(const emulation_devices *device, json condition)
{
    type = interrupt_name_type_map[condition["type"]];
    entryPoint = value_convert::get_address(device, condition["entryPoint"]);
}

interrupt_type condition_interrupt::get_type() const
{
    return type;
}

uint16_t condition_interrupt::get_entry_point() const
{
    return entryPoint;
}
