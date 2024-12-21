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

    vector<debug_segment *> authorized_segments;

    void parse_debug_def(const string &line);

    string get_substr(const string &value, const string &begin, const string &end) const;
    int get_int_substr(const string &value, const string &begin, const string &end, int radix) const;
    void add_file(const string &line);
    void add_segment(const string &line);
    void add_span(const string &line);
    void add_source_line(const string &line);
    void make_address_source_map();
    void add_symbol(const string &line);

public:
    debug_info(args_parser *args, json config);
    string get_path() const;
    void add_line_coverage(uint16_t address);
    string get_source_line(uint16_t address) const;
    bool has_address(const string &label) const;
    string get_label(uint16_t address) const;
    uint16_t get_address(const string &label) const;
    bool has_write_access(uint16_t address) const;
    void test_segment_access(uint16_t address) const;
    map<int, debug_segment *> get_segment_def_map() const;
    debug_segment *get_segment_def(uint16_t address) const;
    debug_segment *get_segment_def(const string &name) const;
    void add_segment_def(int id, const string &name, uint16_t start, int size, bool writable, const string &image_file_name = string(), int image_file_offset = 0);
    void remove_segment_def(int id);
    void remove_segment_def(const string &name);
    void add_authorized_segment_def(uint16_t start, int size);
    void add_authorized_segment_def(const string &name);
    bool has_authorized_access(uint16_t address) const;
    void save_coverage();
};
