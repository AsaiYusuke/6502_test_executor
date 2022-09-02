#include <sstream>
#include "enum/value_type.h"
#include "util/value_convert.h"
#include "emulation/emulation_devices.h"
#include "exception/parse_abort.h"

uint16_t value_convert::get_address(emulation_devices *device, json value)
{
    uint16_t address;
    if (value.is_null())
        return 0;

    if (!value["address"].is_null())
        return parse_json_number(device, value["address"]);

    return device->get_address(value["label"].get<string>()) +
           parse_json_number(device, value["offset"]);
}

uint16_t value_convert::parse_json_number(emulation_devices *device, json value)
{
    switch (value.type())
    {
        case json::value_t::number_integer:
        case json::value_t::number_unsigned:
            return value.get<uint16_t>();
        case json::value_t::string:
        {
            string str = value.get<string>();
            if (str.compare(0, 1, "$") == 0)
                return stoi(str.substr(1), 0, 16);
            else if (str.compare(0, 1, "%") == 0)
                return stoi(str.substr(1), 0, 2);
            break;
        }
        case json::value_t::object:
        {
            auto address = get_address(device, value);
            if (value["type"].is_null())
                return device->get_memory()->read(address);

            if (value["type"].is_string())
                switch (value_name_type_map[value["type"].get<string>()])
                {
                    case value_type::VALUE:
                        return device->get_memory()->read(address);
                    case value_type::HIBYTE:
                        return address >> 8;
                    case value_type::LOBYTE:
                        return address & 0xFF;
                    case value_type::RTS_HIBYTE:
                        return (address - 1) >> 8;
                    case value_type::RTS_LOBYTE:
                        return (address - 1) & 0xFF;
                }
            break;
        }
        case json::value_t::null:
            return 0;
    }
    
    throw invalid_argument("Invalid format: " + to_string(value));
}

uint16_t value_convert::to_two_complement_byte(emulation_devices *device, json value)
{
    auto number = parse_json_number(device, value);
    if (number < 0)
        number += 256;
    return number;
}

string value_convert::to_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << value;
    return ss.str();
}

string value_convert::to_zero_filled_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << setw(4) << setfill('0') << value;
    return ss.str();
}

json value_convert::parse_variable(json definitions_def, json value)
{
    if (value.is_string() && definitions_def.is_object())
    {
        auto valueStr = value.get<string>();
        if (valueStr.front() == '@')
        {
            if (definitions_def.contains(valueStr.substr(1)))
                return definitions_def[valueStr.substr(1)];

            throw invalid_argument("Definition not found: " + valueStr.substr(1));
        }
    }
    return value;
}
