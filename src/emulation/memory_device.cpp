#include "emulation/emulation_devices.h"
#include "util/value_convert.h"
#include "exception/cpu_runtime_error.h"
#include "enum/platform_type.h"

memory_device::memory_device(emulation_devices *_device, args_parser *args, json config)
{
    device = _device;

    string debug_path;
    if (config["debugFile"].is_string())
        debug_path = config["debugFile"].get<string>();
    else
        debug_path = args->get_debug_path();

    debug = new debug_info(debug_path);
    rom = new rom_image(debug);

    if (config["invalidMemory"]["enable"].is_boolean())
        assert_invalid_memory = config["invalidMemory"]["enable"].get<bool>();
    else
        assert_invalid_memory = true;

    vector<int> remove_segment_ids;
    if (config["invalidMemory"]["ignoreList"].is_null())
    {
        switch (rom->detect_platform())
        {
        case platform_type::NES:
            debug->add_segment_def(-1, "CPU_STACK", 0x100, 0x100, true);
            debug->add_segment_def(-1, "NES_PPU_PORTS", 0x2000, 0x2020, true);
            break;
        }

        remove_segment_ids = get_detected_remove_segment_ids();
    }
    else
    {
        for (auto &ignore_def : config["invalidMemory"]["ignoreList"])
        {
            if (ignore_def["start"].is_object() && !ignore_def["size"].is_null())
                debug->add_segment_def(
                    -1,
                    "USER_INFO",
                    value_convert::get_address(device, ignore_def["start"]),
                    value_convert::parse_json_number(device, ignore_def["size"]),
                    true);
            else if (ignore_def["name"].is_string())
                remove_segment_ids.push_back(
                    debug->get_segment_def(ignore_def["name"].get<string>()).get_id());
            else if (ignore_def["detect"].is_boolean() && ignore_def["detect"].get<bool>())
            {
                auto ids = get_detected_remove_segment_ids();
                remove_segment_ids.insert(
                    remove_segment_ids.end(), ids.begin(), ids.end());
            }
        }
    }
    for (auto id : remove_segment_ids)
        debug->remove_segment_def(id);
}

vector<int> memory_device::get_detected_remove_segment_ids()
{
    vector<int> remove_ids;
    switch (rom->detect_platform())
    {
    case platform_type::NES:
        for (auto element : debug->get_segment_def_map())
            if (!element.second.is_nes_cpu_memory())
                remove_ids.push_back(element.first);
        break;
    }
    return remove_ids;
}

void memory_device::clear()
{
    ram.clear();
    read_sequences.clear();
    write_sequences.clear();
    read_counts.clear();
    write_counts.clear();
}

string memory_device::get_source_line(uint16_t address)
{
    return debug->get_source_line(address);
}

uint16_t memory_device::get_address(string label)
{
    if (!debug->has_address(label))
        throw invalid_argument("Symbol not found: " + label);

    return debug->get_address(label);
}

void memory_device::set_read_sequence(uint16_t address, vector<uint8_t> sequence, bool permanent)
{
    read_sequences[address] = sequence;
    read_permanent[address] = permanent;
}

vector<uint8_t> memory_device::get_write_sequence(uint16_t address, size_t length)
{
    if (write_sequences.count(address) > 0)
    {
        vector<uint8_t> result(write_sequences[address].end() - length, write_sequences[address].end());
        return result;
    }
    return {read(address)};
}

uint8_t memory_device::get_read_count(uint16_t address)
{
    return read_counts[address];
}

uint8_t memory_device::get_write_count(uint16_t address)
{
    return write_counts[address];
}

uint8_t memory_device::read(uint16_t address)
{
    read_counts[address]++;

    try
    {
        debug_segment segment_def = debug->get_segment_def(address);
        if (segment_def.is_readonly())
        {
            auto image = rom->get(segment_def.get_id());
            return image[segment_def.get_image_file_address(address)];
        }
    }
    catch (const cpu_runtime_error &e)
    {
        // When exception occurs, it is considered RAM access.
        device->add_error_reuslt(e.get_type(), e.what());
    }

    if (read_sequences.count(address) > 0)
    {
        ram[address] = read_sequences[address].front();

        if (read_sequences[address].size() >= 2 || !read_permanent[address])
        {
            read_sequences[address].erase(read_sequences[address].begin());

            if (read_sequences[address].size() == 0)
                read_sequences.erase(address);
        }
    }

    return ram[address];
}

uint8_t memory_device::read_raw(uint16_t address)
{
    try
    {
        debug_segment segment_def = debug->get_segment_def(address);
        if (segment_def.is_readonly())
        {
            auto image = rom->get(segment_def.get_id());
            return image[segment_def.get_image_file_address(address)];
        }
    }
    catch (const cpu_runtime_error &e)
    {
        // When exception occurs, it is considered RAM access.
    }

    return ram[address];
}

void memory_device::write(uint16_t address, uint8_t value)
{
    write_counts[address]++;
    write_sequences[address].push_back(value);

    if (assert_invalid_memory)
    {
        try
        {
            if (!debug->has_write_access(address))
            {
                device->add_error_reuslt(
                    runtime_error_type::READONLY_MEMORY,
                    "address=" + value_convert::to_zero_filled_hex_string(address));
            }
        }
        catch (const cpu_runtime_error &e)
        {
            device->add_error_reuslt(runtime_error_type::OUT_OF_SEGMENT, e.what());
        }
    }

    ram[address] = value;
}

void memory_device::write_raw(uint16_t address, uint8_t value)
{
    ram[address] = value;
}

void memory_device::print()
{
    printf("MEMORY result:\n");

    int max_label_len = 5;
    for (auto ram_entry : ram)
    {
        int length = debug->get_label(ram_entry.first).length();
        if (max_label_len < length)
            max_label_len = length;
    }

    int max_segment_len = 7;
    for (auto ram_entry : ram)
    {
        uint16_t address = ram_entry.first;
        try
        {
            int length = debug->get_segment_def(address).get_name().length();
            if (max_segment_len < length)
                max_segment_len = length;
        }
        catch (const cpu_runtime_error)
        {
        }
    }

    printf("  Address  %-*s  %-*s  Value\n", max_segment_len + 8, "Segment", max_label_len, "Label");
    printf(
        "  -------  %-*s  %-*s  -----\n",
        max_segment_len, string("------------------------").substr(0, max_segment_len + 8).c_str(),
        max_label_len, string("------------------------").substr(0, max_label_len).c_str());

    for (auto ram_entry : ram)
    {
        uint16_t address = ram_entry.first;
        uint8_t value = ram_entry.second;
        string segment_name = "";
        string segment_start = "     ";
        try
        {
            auto segment_def = debug->get_segment_def(address);
            segment_name = segment_def.get_name();
            segment_start = value_convert::to_zero_filled_hex_string(segment_def.get_start());
        }
        catch(const cpu_runtime_error)
        {
        }
        
        printf(
            "  $%04X    %-*s (%s)  %-*s  $%X\n",
            address,
            max_segment_len,
            segment_name.c_str(),
            segment_start.c_str(),
            max_label_len,
            debug->get_label(address).c_str(),
            value);
    }
}
