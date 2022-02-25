#include <fstream>
#include <vector>

#include "emulation/debug_info.h"
#include "exception/file_open.h"
#include "exception/parse_ignore_entry.h"
#include "exception/cpu_runtime_error.h"
#include "util/value_convert.h"

debug_info::debug_info(string path)
{
    ifstream file(path);
    if (!file.is_open())
        throw file_open_error(path);

    string line;
    while (getline(file, line))
    {
        try
        {
            parse_debug_def(line);
        }
        catch (const parse_ignore_entry)
        {
        }
    }
    file.close();

    make_address_source_map();
}

void debug_info::parse_debug_def(string line)
{
    if (line.substr(0, 5).compare("file\t") == 0)
    {
        add_file(line);
        return;
    }
    if (line.substr(0, 4).compare("seg\t") == 0)
    {
        add_segment(line);
        return;
    }
    if (line.substr(0, 5).compare("span\t") == 0)
    {
        add_span(line);
        return;
    }
    if (line.substr(0, 5).compare("line\t") == 0)
    {
        add_source_line(line);
        return;
    }
    if (line.substr(0, 4).compare("sym\t") == 0)
    {
        add_symbol(line);
        return;
    }
}

string debug_info::get_substr(string value, string begin, string end)
{
    size_t begin_pos;
    if (begin.empty())
        begin_pos = 0;
    else
    {
        begin_pos = value.find(begin);
        if (begin_pos == string::npos)
            throw parse_ignore_entry();

        begin_pos += begin.length();
    }

    size_t str_size;
    if (end.empty())
        str_size = string::npos;
    else
    {
        str_size = value.find(end, begin_pos);
        if (str_size == string::npos)
            throw parse_ignore_entry();

        str_size -= begin_pos;
    }

    return value.substr(begin_pos, str_size);
}

int debug_info::get_int_substr(string value, string begin, string end, int radix)
{
    return stoi(get_substr(value, begin, end), 0, radix);
}

void debug_info::add_file(string line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto name = get_substr(line, ",name=\"", "\"");

    source_file_map[id] = name;
}

void debug_info::add_segment(string line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto name = get_substr(line, ",name=\"", "\"");
    auto start = get_int_substr(line, ",start=0x", ",", 16);
    auto size = get_int_substr(line, ",size=0x", ",", 16);
    bool writable = (line.find(",type=rw") != string::npos);
    bool file_exist = (line.find(",oname=") != string::npos);
    auto ooffs = 0;
    if (file_exist)
        ooffs = get_int_substr(line, ",ooffs=", "", 10);

    add_segment_def(id, name, start, size, writable, file_exist, ooffs);
}

void debug_info::add_span(string line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto segment = get_int_substr(line, ",seg=", ",", 10);
    auto start = get_int_substr(line, ",start=", ",", 10);

    span_map[id] = make_pair(segment, start);
}

void debug_info::add_source_line(string line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto file = get_int_substr(line, ",file=", ",", 10);
    auto line_number = get_int_substr(line, ",line=", ",", 10);
    auto spans = get_substr(line, ",span=", ",");

    size_t pos;
    vector<int> span_ids;
    while ((pos = spans.find("+")) != string::npos)
    {
        span_ids.push_back(stoi(spans.substr(0, pos), 0, 10));
        spans.erase(0, pos + 1);
    }
    span_ids.push_back(stoi(spans, 0, 10));

    source_line_map[id] = make_tuple(file, line_number, span_ids);
}

void debug_info::add_symbol(string line)
{
    auto label = get_substr(line, ",name=\"", "\"");
    auto value = get_int_substr(line, ",val=0x", ",", 16);

    label_address_map[label] = value;
    address_label_map[value] = label;
}

void debug_info::make_address_source_map()
{
    for (auto line_def : source_line_map)
    {
        auto file_id = get<0>(line_def.second);
        auto line_number = get<1>(line_def.second);
        auto span_ids = get<2>(line_def.second);
        auto file_name = source_file_map[file_id];

        for (auto span_id : span_ids)
        {
            auto span_def = span_map[span_id];
            auto segment_id = span_def.first;
            uint16_t span_addr = span_def.second;
            if (segment_def_map.count(segment_id) == 0)
                continue;
            uint16_t segment_addr = segment_def_map.at(segment_id).get_start();
            address_source_map[segment_addr + span_addr] = make_pair(file_name, line_number);
        }
    }
}

string debug_info::get_source_line(uint16_t address)
{
    if (address_source_map.count(address) == 0)
        return "";

    auto line_def = address_source_map[address];
    return line_def.first + ":" + to_string(line_def.second);
}

bool debug_info::has_address(string label)
{
    return label_address_map.count(label) > 0;
}

string debug_info::get_label(uint16_t address)
{
    return address_label_map[address];
}

uint16_t debug_info::get_address(string label)
{
    return label_address_map[label];
}

bool debug_info::has_write_access(uint16_t address)
{
    return get_segment_def(address).is_writable();
}

bool debug_info::has_read_access(uint16_t address)
{
    get_segment_def(address);
    return true;
}

debug_segment debug_info::get_segment_def(uint16_t address)
{
    for (auto element : segment_def_map)
    {
        auto segment_def = element.second;
        if (segment_def.contains(address))
            return segment_def;
    }
    throw cpu_runtime_error(runtime_error_type::OUT_OF_SEGMENT, "address=" + value_convert::to_zero_filled_hex_string(address));
}

debug_segment debug_info::get_segment_def(string name)
{
    for (auto element : segment_def_map)
    {
        if (element.second.get_name() == name)
        {
            return element.second;
        }
    }
    throw cpu_runtime_error(runtime_error_type::OUT_OF_SEGMENT, "name=" + name);
}

void debug_info::add_segment_def(
    int id, string name, uint16_t start, int size, bool writable, bool image_file_exist, int image_file_offset)
{
    if (id < 0)
    {
        int max_id = 0;
        for (auto element : segment_def_map)
        {
            if (max_id < element.first)
                max_id = element.first;
        }
        id = max_id + 1;
    }

    segment_def_map.emplace(
        id,
        debug_segment(
            id,
            name,
            start,
            size,
            writable,
            image_file_exist,
            image_file_offset));
}

void debug_info::remove_segment_def(string name)
{
    segment_def_map.erase(get_segment_def(name).get_id());
}

void debug_info::remove_detected_segment(string type)
{
    if (type != "NES")
        return;

    vector<int> remove_ids;
    for (auto element : segment_def_map)
    {
        auto segment_def = element.second;
        if (segment_def.is_nes_cpu_memory())
            continue;
        remove_ids.push_back(element.first);
    }
    for (auto id : remove_ids)
        segment_def_map.erase(id);
}
