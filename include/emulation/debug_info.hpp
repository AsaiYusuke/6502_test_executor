#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class args_parser;
class debug_segment;
class debug_source_line;

class debug_info
{
private:
    string path;
    string test_path;
    string coverage_path;
    set<string> coverage_segment_names;
    map<int, string> source_file_map;
    map<int, debug_segment *> segment_def_map;
    map<int, pair<int, int>> span_map;
    map<int, tuple<int, int, vector<int>>> source_line_map;

    map<uint16_t, debug_source_line *> address_source_map;
    map<int, map<int, map<int, debug_source_line *>>> file_segment_line_source_map;

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
    debug_info(args_parser *args, json config);
    string get_path();
    void add_line_coverage(uint16_t address);
    string get_source_line(uint16_t address);
    bool has_address(string label);
    string get_label(uint16_t address);
    uint16_t get_address(string label);
    bool has_write_access(uint16_t address);
    bool has_read_access(uint16_t address);
    map<int, debug_segment *> get_segment_def_map();
    debug_segment *get_segment_def(uint16_t address);
    debug_segment *get_segment_def(string name);
    void add_segment_def(int id, string name, uint16_t start, int size, bool writable, string image_file_name = string(), int image_file_offset = 0);
    void remove_segment_def(int id);
    void remove_segment_def(string name);
    void save_coverage();
};
