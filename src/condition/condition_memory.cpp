#include <sstream>
#include "util/value_convert.h"
#include "condition/condition_memory.h"

condition_memory::condition_memory(emulation_devices *_device, json condition)
{
    auto address = value_convert::get_address(_device, condition);

    bool is_permanent = condition.value("permanent", false);

    auto sequences = normalize_value_sequences(_device, condition);
    for (decltype(sequences.size()) offset = 0, size = sequences.size(); offset < size; offset++)
    {
        auto sequence = sequences.at(offset);
        value_sequences.push_back(
            condition_memory_value(
                _device,
                address + offset,
                sequence,
                is_permanent,
                create_address_name(_device, condition, offset)));
    }

    auto r_counts = normalize_read_counts(_device, condition);
    for (decltype(r_counts.size()) offset = 0, size = r_counts.size(); offset < size; offset++)
    {
        auto read_count = r_counts.at(offset);
        read_counts.push_back(
            condition_memory_count(
                address + offset,
                read_count,
                create_address_name(_device, condition, offset)));
    }

    auto w_counts = normalize_write_counts(_device, condition);
    for (decltype(w_counts.size()) offset = 0, size = w_counts.size(); offset < size; offset++)
    {
        auto write_count = w_counts.at(offset);
        write_counts.push_back(
            condition_memory_count(
                address + offset,
                write_count,
                create_address_name(_device, condition, offset)));
    }
}

vector<condition_memory_value> condition_memory::get_value_sequences()
{
    return value_sequences;
}

vector<condition_memory_count> condition_memory::get_read_counts()
{
    return read_counts;
}

vector<condition_memory_count> condition_memory::get_write_counts()
{
    return write_counts;
}

string condition_memory::create_address_name(emulation_devices *device, json memory_def, int offset)
{
    bool append_total_address = false;
    stringstream ss;
    uint16_t address = value_convert::get_address(device, memory_def) + offset;

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
            offset += value_convert::parse_json_number(device, memory_def["offset"]);
        }
    }

    if (offset != 0)
        ss << " + " << value_convert::to_hex_string(offset);

    if (offset != 0 || append_total_address)
        ss << " (" << value_convert::to_zero_filled_hex_string(address) << ")";

    return ss.str();
}

json condition_memory::normalize_value_sequences(emulation_devices *_device, json memory_def)
{
    if (memory_def.contains("sequence"))
        if (memory_def["sequence"].at(0).is_array())
            return memory_def["sequence"];
        else
            return {memory_def["sequence"]};
    
    if (memory_def.contains("value"))
        if (memory_def["value"].is_array())
        {
            json value_defs = {};
            for (json &value_def : memory_def["value"])
                value_defs.push_back({value_def});
            return value_defs;
        }
        else
            return {{memory_def["value"]}};

    return {};
}

json condition_memory::normalize_read_counts(emulation_devices *_device, json memory_def)
{
    if (!memory_def.contains("readCount"))
        return {};

    if (memory_def["readCount"].is_array())
        return memory_def["readCount"];
    else
        return {memory_def["readCount"]};
}

json condition_memory::normalize_write_counts(emulation_devices *_device, json memory_def)
{
    if (!memory_def.contains("writeCount"))
        return {};

    if (memory_def["writeCount"].is_array())
        return memory_def["writeCount"];
    else
        return {memory_def["writeCount"]};
}
