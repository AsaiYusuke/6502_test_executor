#include <sstream>

#include "condition/condition_memory.h"

condition_memory::condition_memory(emulation_devices *_device, json condition)
{
    if (condition.is_null())
        return;

    for (auto &memory_def : condition)
    {
        uint16_t address = address_convert::get_address(_device, memory_def);
        uint8_t value = address_convert::to_byte(_device, memory_def["value"]);
        memory_defs.push_back(
            make_tuple(
                address,
                value,
                get_address_name(_device, memory_def)));
    }
}

vector<tuple<uint16_t, uint8_t, string>> condition_memory::get_memory_defs()
{
    return memory_defs;
}

string condition_memory::get_address_name(emulation_devices *device, json memory_def)
{
    if (memory_def.contains("address"))
    {
        return memory_def["address"].get<string>();
    }
    else
    {
        uint16_t address = address_convert::get_address(device, memory_def);
        stringstream ss;
        ss << memory_def["label"].get<string>();

        if (!memory_def["offset"].is_null())
        {
            uint8_t offset = address_convert::to_byte(device, memory_def["offset"]);
            if (offset != 0)
                ss << " + " << to_hex_string(offset);
        }

        ss << " (" << to_hex_string(address) << ")";
        return ss.str();
    }
}

string condition_memory::to_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << value;
    return ss.str();
}
