#pragma once

#include <map>
#include <string>

#include "emulation/debug_segment.h"

using namespace std;

class debug_info
{
private:
    map<int, string> source_file_map;
    map<int, debug_segment> segment_def_map;
    map<int, pair<int, int>> span_map;
    map<int, tuple<int, int, vector<int>>> source_line_map;

    map<uint16_t, pair<string, int>> address_source_map;

    map<string, uint16_t> label_address_map;
    map<uint16_t, string> address_label_map;

    void parse_debug_def(string line);

    string get_substr(string value, string begin, string end);
    int get_int_substr(string value, string begin, string end, int radix);
    void add_file(string line);
    void add_segment(string line);
    void add_span(string line);
    void add_source_line(string line);
    void make_address_source_map();
    void add_symbol(string line);

public:
    debug_info(string debug_path);
    string get_source_line(uint16_t address);
    bool has_address(string label);
    string get_label(uint16_t address);
    uint16_t get_address(string label);
    bool has_write_access(uint16_t address);
    bool has_read_access(uint16_t address);
    debug_segment get_segment_def(uint16_t address);
    debug_segment get_segment_def(string name);
    void add_segment_def(int id, string name, uint16_t start, int size, bool writable, bool image_file_exist = false, int image_file_offset = 0);
    void remove_segment_def(string name);
    void remove_detected_segment(string type);
};
