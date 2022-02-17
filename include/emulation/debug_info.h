#pragma once

#include <map>
#include <string>

using namespace std;

class debug_info
{
private:
    bool omit;
    map<int, string> source_file_map;
    map<int, tuple<uint16_t, int, bool>> segment_map;
    map<string, int> segment_name_id_map;
    map<string, bool> segment_name_file_exist_map;
    map<int, pair<int, int>> span_map;
    map<int, tuple<int, int, vector<int>>> source_line_map;

    map<uint16_t, pair<string, int>> address_source_map;

    map<string, uint16_t> label_address_map;
    map<uint16_t, string> address_label_map;

    void parse_debug_def(string line);
    void add_file(string line);
    void add_segment(string line);
    void add_span(string line);
    void add_source_line(string line);
    void make_address_source_map();
    void add_symbol(string line);

public:
    debug_info(string debug_path);
    bool is_omit();
    string get_source_line(uint16_t address);
    bool has_address(string label);
    string get_label(uint16_t address);
    uint16_t get_address(string label);
    bool has_write_access(uint16_t address);
    bool has_read_access(uint16_t address);
    tuple<uint16_t, int, bool> get_segment_def(uint16_t address);
    void add_segment_def(int id, uint16_t start, int size, bool writable);
    void remove_segment_def(string name);
    void remove_detected_segment(string type);
};
