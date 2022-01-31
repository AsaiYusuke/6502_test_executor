#include <sstream>
#include "condition/address_convert.h"
#include "condition/condition_memory.h"

condition_memory::condition_memory(emulation_devices *_device, json condition)
{
    if (condition.is_null())
        return;

    for (json &memory_def : condition)
    {
        uint16_t address = address_convert::get_address(_device, memory_def);

        auto sequences = get_value_sequences(_device, memory_def);
        for (decltype(sequences.size()) offset = 0, size = sequences.size(); offset < size; offset++)
        {
            auto sequence = sequences.at(offset);
            memory_value_defs.push_back(
                make_tuple(
                    address + offset,
                    sequence,
                    get_address_name(_device, memory_def, offset)));
        }

        auto read_counts = get_read_counts(_device, memory_def);
        for (decltype(read_counts.size()) offset = 0, size = read_counts.size(); offset < size; offset++)
        {
            auto read_count = read_counts.at(offset);
            memory_read_count_defs.push_back(
                make_tuple(
                    address + offset,
                    read_count,
                    get_address_name(_device, memory_def, offset)));
        }

        auto write_counts = get_write_counts(_device, memory_def);
        for (decltype(write_counts.size()) offset = 0, size = write_counts.size(); offset < size; offset++)
        {
            auto write_count = write_counts.at(offset);
            memory_write_count_defs.push_back(
                make_tuple(
                    address + offset,
                    write_count,
                    get_address_name(_device, memory_def, offset)));
        }
    }
}

vector<tuple<uint16_t, vector<uint8_t>, string>> condition_memory::get_memory_value_defs()
{
    return memory_value_defs;
}

vector<tuple<uint16_t, uint8_t, string>> condition_memory::get_memory_read_count_defs()
{
    return memory_read_count_defs;
}

vector<tuple<uint16_t, uint8_t, string>> condition_memory::get_memory_write_count_defs()
{
    return memory_write_count_defs;
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
        ss << " + " << address_convert::to_hex_string(offset);

    if (offset != 0 || append_total_address)
        ss << " (" << address_convert::to_hex_string(address) << ")";

    return ss.str();
}

vector<vector<uint8_t>> condition_memory::get_value_sequences(emulation_devices *_device, json memory_def)
{
    vector<vector<uint8_t>> result;

    json value_defs;
    if (memory_def.contains("sequence"))
    {
        if (memory_def["sequence"].at(0).is_array())
        {
            value_defs = memory_def["sequence"];
        }
        else
        {
            value_defs = {memory_def["sequence"]};
        }
    }
    else if (memory_def.contains("value"))
    {
        if (memory_def["value"].is_array())
        {
            value_defs = {};
            for (json &value_def : memory_def["value"])
            {
                value_defs.push_back({value_def});
            }
        }
        else
            value_defs = {{memory_def["value"]}};
    }
    else
        return {};

    for (json &value_def : value_defs)
    {
        vector<uint8_t> sequence;
        for (json &value_sequence : value_def)
        {
            sequence.push_back(
                address_convert::to_two_complement_byte(_device, value_sequence));
        }
        result.push_back(sequence);
    }
    return result;
}

vector<uint8_t> condition_memory::get_read_counts(emulation_devices *_device, json memory_def)
{
    vector<uint8_t> result;

    json count_defs;
    if (memory_def.contains("readCount"))
    {
        if (memory_def["readCount"].is_array())
        {
            count_defs = memory_def["readCount"];
        }
        else
        {
            count_defs = {memory_def["readCount"]};
        }
    }
    else
        return {};

    for (json &count_def : count_defs)
        result.push_back(count_def.get<uint8_t>());

    return result;
}

vector<uint8_t> condition_memory::get_write_counts(emulation_devices *_device, json memory_def)
{
    vector<uint8_t> result;

    json count_defs;
    if (memory_def.contains("writeCount"))
    {
        if (memory_def["writeCount"].is_array())
        {
            count_defs = memory_def["writeCount"];
        }
        else
        {
            count_defs = {memory_def["writeCount"]};
        }
    }
    else
        return {};

    for (json &count_def : count_defs)
        result.push_back(count_def.get<uint8_t>());

    return result;
}
