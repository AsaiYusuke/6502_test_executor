#include <sstream>

#include "condition/condition_memory.h"

condition_memory::condition_memory(emulation_devices *_device, json condition)
{
    if (condition.is_null())
        return;

    for (json &memory_def : condition)
    {
        uint16_t address = address_convert::get_address(_device, memory_def);
        vector<uint8_t> values = get_values(_device, memory_def);
        for (int offset = 0; offset < values.size(); offset++)
        {
            uint8_t value = values.at(offset);
            memory_defs.push_back(
                make_tuple(
                    address + offset,
                    value,
                    get_address_name(_device, memory_def, offset)));
        }
    }
}

vector<tuple<uint16_t, uint8_t, string>> condition_memory::get_memory_defs()
{
    return memory_defs;
}

string condition_memory::get_address_name(emulation_devices *device, json memory_def, int offset)
{
    bool append_total_address = false;
    stringstream ss;
    uint16_t address = address_convert::get_address(device, memory_def) + offset;

    if (memory_def.contains("address"))
    {
        ss << memory_def["address"].get<string>();
    }
    else
    {
        append_total_address = true;
        ss << memory_def["label"].get<string>();

        if (!memory_def["offset"].is_null())
        {
            offset += address_convert::to_byte(device, memory_def["offset"]);
        }
    }

    if (offset != 0)
        ss << " + " << to_hex_string(offset);

    if (offset != 0 || append_total_address)
        ss << " (" << to_hex_string(address) << ")";

    return ss.str();
}

string condition_memory::to_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << value;
    return ss.str();
}

vector<uint8_t> condition_memory::get_values(emulation_devices *_device, json memory_def)
{
    vector<uint8_t> result;

    if (memory_def.contains("value"))
    {
        result.push_back(
            address_convert::to_byte(_device, memory_def["value"]));
        return result;
    }

    for (json &value_json : memory_def["values"])
    {
        result.push_back(
            address_convert::to_byte(_device, value_json));
    }
    return result;
}
