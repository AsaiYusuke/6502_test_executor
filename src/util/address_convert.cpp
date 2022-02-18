#include <sstream>
#include "util/address_convert.h"

uint16_t address_convert::get_address(emulation_devices *device, json value)
{
    uint16_t address;
    if (!value["address"].is_null())
        return to_byte(device, value["address"]);

    return device->get_address(value["label"].get<string>()) +
           to_byte(device, value["offset"]);
}

uint16_t address_convert::to_byte(emulation_devices *device, json value)
{
    switch (value.type())
    {
    case json::value_t::number_integer:
    case json::value_t::number_unsigned:
        return value.get<uint16_t>();
    case json::value_t::string:
        string str = value.get<string>();
        if (str.compare(0, 1, "$") == 0)
            return stoi(str.substr(1), 0, 16);
        else if (str.compare(0, 1, "%") == 0)
            return stoi(str.substr(1), 0, 2);
    }
    return 0;
}

uint16_t address_convert::to_two_complement_byte(emulation_devices *device, json value)
{
    auto number = to_byte(device, value);
    if (number < 0)
        number += 256;
    return number;
}

string address_convert::to_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << value;
    return ss.str();
}

string address_convert::to_zero_filled_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << setw(4) << setfill('0') << value;
    return ss.str();
}
