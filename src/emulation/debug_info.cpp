#include "emulation/debug_info.hpp"

#include <fstream>
#include <vector>

#include "args_parser.hpp"
#include "emulation/debug_segment.hpp"
#include "emulation/debug_source_line.hpp"
#include "exception/cpu_runtime_error.hpp"
#include "exception/file_open.hpp"
#include "exception/parse_ignore_entry.hpp"
#include "util/value_convert.hpp"

using namespace std;

debug_info::debug_info(args_parser *args, json config)
{
    if (config["debugFile"].is_string())
        path = config["debugFile"].get<string>();
    else
        path = args->get_debug_path();

    test_path = args->get_test_path();

    coverage_path = args->get_coverage_path();
    stringstream segment_stringstream{args->get_coverage_segment_names()};
    string buf;
    while (getline(segment_stringstream, buf, ':'))
        coverage_segment_names.emplace(buf);

    ifstream file(path);
    if (!file.is_open())
        throw file_open_error(path);

    string line;
    while (getline(file, line))
        try
        {
            parse_debug_def(line);
        }
        catch (const parse_ignore_entry)
        {
        }

    file.close();

    make_address_source_map();
}

string debug_info::get_path() const
{
    return path;
}

void debug_info::parse_debug_def(const string &line)
{
    if (line.substr(0, 5).compare("file\t") == 0)
        add_file(line);
    else if (line.substr(0, 4).compare("seg\t") == 0)
        add_segment(line);
    else if (line.substr(0, 5).compare("span\t") == 0)
        add_span(line);
    else if (line.substr(0, 5).compare("line\t") == 0)
        add_source_line(line);
    else if (line.substr(0, 4).compare("sym\t") == 0)
        add_symbol(line);
}

string debug_info::get_substr(const string &value, const string &begin, const string &end) const
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

int debug_info::get_int_substr(const string &value, const string &begin, const string &end, int radix) const
{
    return stoi(get_substr(value, begin, end), 0, radix);
}

void debug_info::add_file(const string &line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto name = get_substr(line, ",name=\"", "\"");

    source_file_map[id] = name;
}

void debug_info::add_segment(const string &line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto name = get_substr(line, ",name=\"", "\"");
    auto start = get_int_substr(line, ",start=0x", ",", 16);
    auto size = get_int_substr(line, ",size=0x", ",", 16);
    bool writable = (line.find(",type=rw") != string::npos);
    auto image_file_name = string();
    auto ooffs = 0;
    if (line.find(",oname=\"") != string::npos)
    {
        image_file_name = get_substr(line, ",oname=\"", "\"");
        ooffs = get_int_substr(line, ",ooffs=", "", 10);
    }
    add_segment_def(id, name, start, size, writable, image_file_name, ooffs);
}

void debug_info::add_span(const string &line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto segment = get_int_substr(line, ",seg=", ",", 10);
    auto start = get_int_substr(line, ",start=", ",", 10);

    span_map[id] = make_pair(segment, start);
}

void debug_info::add_source_line(const string &line)
{
    auto id = get_int_substr(line, "\tid=", ",", 10);
    auto file = get_int_substr(line, ",file=", ",", 10);
    auto line_number = get_int_substr(line, ",line=", ",", 10);
    auto spans = get_substr(line, ",span=", "");

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

void debug_info::add_symbol(const string &line)
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

        for (auto span_id : span_ids)
        {
            auto span_def = span_map[span_id];
            auto segment_id = span_def.first;
            uint16_t span_addr = span_def.second;
            if (segment_def_map.count(segment_id) == 0)
                continue;
            uint16_t segment_addr = segment_def_map.at(segment_id)->get_start();
            uint16_t address = segment_addr + span_addr;

            if (address_source_map.count(address) > 0)
                continue;

            if (file_segment_line_source_map[file_id][segment_id].count(line_number) == 0)
                address_source_map[address] = new debug_source_line(file_id, segment_id, line_number);
            else
                address_source_map[address] = file_segment_line_source_map[file_id][segment_id][line_number];

            file_segment_line_source_map[file_id][segment_id][line_number] = address_source_map[address];
        }
    }
}

void debug_info::add_line_coverage(uint16_t address)
{
    if (address_source_map.count(address) == 0)
        return;
    address_source_map[address]->add_cover();
}

string debug_info::get_source_line(uint16_t address) const
{
    if (address_source_map.count(address) == 0)
        return "";

    auto line_def = address_source_map.at(address);
    auto file = source_file_map.at(line_def->get_file_id());
    auto lineNumber = to_string(line_def->get_line_number());
    return file + ":" + lineNumber;
}

bool debug_info::has_address(const string &label) const
{
    return label_address_map.count(label) > 0;
}

string debug_info::get_label(uint16_t address) const
{
    if (address_label_map.count(address) == 0)
        return "";

    return address_label_map.at(address);
}

uint16_t debug_info::get_address(const string &label) const
{
    return label_address_map.at(label);
}

bool debug_info::has_write_access(uint16_t address) const
{
    return get_segment_def(address)->is_writable();
}

void debug_info::test_segment_access(uint16_t address) const
{
    get_segment_def(address);
}

map<int, debug_segment *> debug_info::get_segment_def_map() const
{
    return segment_def_map;
}

debug_segment *debug_info::get_segment_def(uint16_t address) const
{
    for (auto element : segment_def_map)
        if (element.second->contains(address))
            return element.second;
    throw cpu_runtime_error(runtime_error_type::OUT_OF_SEGMENT, "address=" + value_convert::to_zero_filled_hex_string(address));
}

debug_segment *debug_info::get_segment_def(const string &name) const
{
    for (auto element : segment_def_map)
        if (element.second->get_name() == name)
            return element.second;
    throw cpu_runtime_error(runtime_error_type::OUT_OF_SEGMENT, "name=" + name);
}

void debug_info::add_segment_def(
    int id, const string &name, uint16_t start, int size, bool writable, const string &image_file_name, int image_file_offset)
{
    if (id < 0)
        id = segment_def_map.rbegin()->first + 1;

    segment_def_map.emplace(
        id,
        new debug_segment(
            id,
            name,
            start,
            size,
            writable,
            image_file_name,
            image_file_offset));
}

void debug_info::remove_segment_def(int id)
{
    segment_def_map.erase(id);
}

void debug_info::remove_segment_def(const string &name)
{
    segment_def_map.erase(get_segment_def(name)->get_id());
}

void debug_info::add_authorized_segment_def(uint16_t start, int size)
{
    authorized_segments.push_back(
        new debug_segment(-1, "AUTHORIZED_SEGMENT", start, size, false, "", 0));
}

void debug_info::add_authorized_segment_def(const string &name)
{
    authorized_segments.push_back(
        get_segment_def(name));
}

bool debug_info::has_authorized_access(uint16_t address) const
{
    for (auto segment_def : authorized_segments)
        if (segment_def->contains(address))
            return true;

    return false;
}

void debug_info::save_coverage()
{
    if (test_path.empty() || coverage_path.empty() || coverage_segment_names.empty())
        return;

    ofstream file(coverage_path, ios::app);

    if (!file.is_open())
        throw file_open_error(coverage_path);

    file << "TN:" << test_path << endl;
    for (auto file_pair : file_segment_line_source_map)
    {
        file << "SF:" << source_file_map[file_pair.first] << endl;

        for (auto segment_pair : file_pair.second)
        {
            if (segment_def_map.count(segment_pair.first) == 0)
                continue;

            if (coverage_segment_names.count(segment_def_map[segment_pair.first]->get_name()) == 0)
                continue;

            for (auto debug_line : segment_pair.second)
                file << "DA:"
                     << debug_line.second->get_line_number()
                     << ","
                     << (debug_line.second->get_cover())
                     << endl;
        }
    }
    file << "end_of_record" << endl;

    file.close();
}
