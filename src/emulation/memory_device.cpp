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

    if (config["invalidMemory"]["enable"].is_null())
        assert_invalid_memory = true;
    else
        assert_invalid_memory = config["invalidMemory"]["enable"].get<bool>();

    if (config["invalidMemory"]["ignoreList"].is_null())
    {
        debug->add_segment_def(-1, "CPU_STACK", 0x100, 0xFF, true);
        debug->add_segment_def(-1, "NES_PPU_PORTS", 0x2000, 0x2020, true);
        for (auto id : get_detected_remove_segment_ids())
            debug->remove_segment_def(id);
    }
    else
    {
        vector<int> remove_segment_ids;
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
        for (auto id : remove_segment_ids)
        {
            debug->remove_segment_def(id);
        }
    }
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

string memory_device::get_label(uint16_t address)
{
    return debug->get_label(address);
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

void memory_device::print()
{
    printf("MEMORY result:\n");

    int max_length = 0;
    for (auto ram_entry : ram)
    {
        int length = debug->get_label(ram_entry.first).length();
        if (max_length < length)
            max_length = length;
    }
    if (max_length < 5)
        max_length = 5;

    printf("  Address  %-*s  Value\n", max_length, "Label");
    printf("  -------  %-*s  -----\n", max_length, "-----");
    for (auto ram_entry : ram)
    {
        uint16_t k = ram_entry.first;
        uint8_t v = ram_entry.second;
        printf("  $%04X    %-*s  $%X\n", k, max_length, debug->get_label(k).c_str(), v);
    }
}
