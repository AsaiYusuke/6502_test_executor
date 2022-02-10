#include <sstream>
#include "util/address_convert.h"

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

    return value.get<uint8_t>();
}

uint8_t address_convert::to_two_complement_byte(emulation_devices *device, json value)
{
    return device->two_complement_byte(address_convert::to_byte(device, value));
}

string address_convert::to_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << value;
    return ss.str();
}
