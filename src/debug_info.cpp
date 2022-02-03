#include <fstream>
#include <vector>

#include "debug_info.h"
#include "exception/file_open.h"

debug_info::debug_info(string path)
{
    ifstream file(path);
    if (!file.is_open())
        throw file_open_error(path);

    string line;
    while (getline(file, line))
        parse_debug_def(line);
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

void debug_info::add_file(string line)
{
    auto id_pos = line.find("\tid=");
    if (id_pos == string::npos)
        return;

    id_pos += 4;

    auto id_end_pos = line.find(",", id_pos);
    if (id_end_pos == string::npos)
        return;

    auto name_pos = line.find(",name=\"");
    if (name_pos == string::npos)
        return;

    name_pos += 7;

    auto name_end_pos = line.find("\"", name_pos);
    if (name_end_pos == string::npos)
        return;

    auto id = stoi(line.substr(id_pos, id_end_pos - id_pos), 0, 10);
    auto name = line.substr(name_pos, name_end_pos - name_pos);

    source_file_map[id] = name;
}

void debug_info::add_segment(string line)
{
    auto id_pos = line.find("\tid=");
    if (id_pos == string::npos)
        return;

    id_pos += 4;

    auto id_end_pos = line.find(",", id_pos);
    if (id_end_pos == string::npos)
        return;

    auto start_pos = line.find(",start=0x");
    if (start_pos == string::npos)
        return;

    start_pos += 9;

    auto start_end_pos = line.find(",", start_pos);
    if (start_end_pos == string::npos)
        return;

    auto id = stoi(line.substr(id_pos, id_end_pos - id_pos), 0, 10);
    auto start = stoi(line.substr(start_pos, start_end_pos - start_pos), 0, 16);

    segment_map[id] = start;
}

void debug_info::add_span(string line)
{
    auto id_pos = line.find("\tid=");
    if (id_pos == string::npos)
        return;

    id_pos += 4;

    auto id_end_pos = line.find(",", id_pos);
    if (id_end_pos == string::npos)
        return;

    auto segment_pos = line.find(",seg=");
    if (segment_pos == string::npos)
        return;

    segment_pos += 5;

    auto segment_end_pos = line.find(",", segment_pos);
    if (segment_end_pos == string::npos)
        return;

    auto start_pos = line.find(",start=");
    if (start_pos == string::npos)
        return;

    start_pos += 7;

    auto start_end_pos = line.find(",", start_pos);
    if (start_end_pos == string::npos)
        return;

    auto id = stoi(line.substr(id_pos, id_end_pos - id_pos), 0, 10);
    auto segment = stoi(line.substr(segment_pos, segment_end_pos - segment_pos), 0, 10);
    auto start = stoi(line.substr(start_pos, start_end_pos - start_pos), 0, 10);

    span_map[id] = make_pair(segment, start);
}

void debug_info::add_source_line(string line)
{
    auto id_pos = line.find("\tid=");
    if (id_pos == string::npos)
        return;

    id_pos += 4;

    auto id_end_pos = line.find(",", id_pos);
    if (id_end_pos == string::npos)
        return;

    auto file_pos = line.find(",file=");
    if (file_pos == string::npos)
        return;

    file_pos += 6;

    auto file_end_pos = line.find(",", file_pos);
    if (file_end_pos == string::npos)
        return;

    auto line_pos = line.find(",line=");
    if (line_pos == string::npos)
        return;

    line_pos += 6;

    auto line_end_pos = line.find(",", line_pos);
    if (line_end_pos == string::npos)
        return;

    auto span_pos = line.find(",span=");
    if (span_pos == string::npos)
        return;

    span_pos += 6;

    auto span_end_pos = line.find(",", span_pos);
    if (span_end_pos == string::npos)
        span_end_pos = line.length();

    auto id = stoi(line.substr(id_pos, id_end_pos - id_pos), 0, 10);
    auto file = stoi(line.substr(file_pos, file_end_pos - file_pos), 0, 10);
    auto line_number = stoi(line.substr(line_pos, line_end_pos - line_pos), 0, 10);

    auto spans = line.substr(span_pos, span_end_pos - span_pos);

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
    auto name_pos = line.find(",name=\"");
    if (name_pos == string::npos)
        return;

    name_pos += 7;

    auto name_end_pos = line.find("\"", name_pos);
    if (name_end_pos == string::npos)
        return;

    auto val_pos = line.find(",val=0x");
    if (val_pos == string::npos)
        return;

    val_pos += 7;

    auto val_end_pos = line.find(",", val_pos);
    if (val_end_pos == string::npos)
        return;

    auto label = line.substr(name_pos, name_end_pos - name_pos);
    auto value = stoi(line.substr(val_pos, val_end_pos - val_pos), 0, 16);

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
            uint16_t segment_addr = segment_map[segment_id];
            address_source_map[segment_addr + span_addr] = make_pair(file_name, line_number);
        }
    }
}

string debug_info::get_source_line(uint16_t address)
{
    auto line_def = address_source_map[address];
    return line_def.first + ":" + to_string(line_def.second);
}

bool debug_info::has_label(string label)
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
