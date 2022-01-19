#include "condition/address_convert.h"

uint16_t address_convert::get_address(emulation_devices *device, json value)
{
    uint16_t address;
    if (!value["address"].is_null())
        return to_byte(device, value["address"]);

    return device->get_address(
        value["label"].get<string>(),
        to_byte(device, value["offset"]));
}

uint8_t address_convert::to_byte(emulation_devices *device, json value)
{
    if (value.is_null())
        return 0;

    if (value.is_string())
        return device->to_byte(value.get<string>());

    return device->two_complement_byte(value.get<uint8_t>());
}
