#include "condition/condition_memory.hpp"
#include "condition/condition_memory_value.hpp"
#include "condition/condition_memory_count.hpp"
#include "util/value_convert.hpp"

condition_memory::condition_memory(emulation_devices *device, json condition)
{
    auto address = value_convert::get_address(device, condition);

    bool is_permanent = condition.value("permanent", false);

    auto sequences = normalize_value_sequences(device, condition);
    for (decltype(sequences.size()) offset = 0, size = sequences.size(); offset < size; offset++)
    {
        auto sequence = sequences.at(offset);
        value_sequences.push_back(
            new condition_memory_value(
                device,
                address + offset,
                sequence,
                is_permanent,
                create_address_name(device, condition, offset)));
    }

    auto r_counts = normalize_read_counts(device, condition);
    for (decltype(r_counts.size()) offset = 0, size = r_counts.size(); offset < size; offset++)
    {
        auto read_count = r_counts.at(offset);
        read_counts.push_back(
            new condition_memory_count(
                device,
                address + offset,
                read_count,
                create_address_name(device, condition, offset)));
    }

    auto w_counts = normalize_write_counts(device, condition);
    for (decltype(w_counts.size()) offset = 0, size = w_counts.size(); offset < size; offset++)
    {
        auto write_count = w_counts.at(offset);
        write_counts.push_back(
            new condition_memory_count(
                device,
                address + offset,
                write_count,
                create_address_name(device, condition, offset)));
    }
}

vector<condition_memory_value *> condition_memory::get_value_sequences()
{
    return value_sequences;
}

vector<condition_memory_count *> condition_memory::get_read_counts()
{
    return read_counts;
}

vector<condition_memory_count *> condition_memory::get_write_counts()
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

json condition_memory::normalize_value_sequences(emulation_devices *device, json memory_def)
{
    if (memory_def.contains("sequence"))
    {
        json sequence = memory_def["sequence"];
        if (sequence.at(0).is_array())
            return sequence;
        else
            return {sequence};
    }

    if (memory_def.contains("value"))
    {
        json value = memory_def["value"];
        if (value.is_array())
        {
            json value_defs = {};
            for (json &value_def : value)
                value_defs.push_back({value_def});
            return value_defs;
        }
        else
            return {{value}};
    }

    return {};
}

json condition_memory::normalize_read_counts(emulation_devices *device, json memory_def)
{
    if (!memory_def.contains("readCount"))
        return {};

    json value = memory_def["readCount"];

    if (value.is_array())
        return value;
    else
        return {value};
}

json condition_memory::normalize_write_counts(emulation_devices *device, json memory_def)
{
    if (!memory_def.contains("writeCount"))
        return {};

    json value = memory_def["writeCount"];

    if (value.is_array())
        return value;
    else
        return {value};
}
