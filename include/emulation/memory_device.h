#pragma once

#include <stdio.h>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "args_parser.h"
#include "memory_access.h"
#include "debug_info.h"

using namespace std;

using json = nlohmann::json;

class memory_device : public i_memory_access
{
private:
    char *rom;
    map<uint16_t, uint8_t> ram;

    map<uint16_t, vector<uint8_t>> read_sequences;
    map<uint16_t, bool> read_permanent;
    map<uint16_t, vector<uint8_t>> write_sequences;

    map<uint16_t, uint8_t> read_counts;
    map<uint16_t, uint8_t> write_counts;

    debug_info *debug;

    void load_rom_image(string path);
    uint16_t memory_offset(uint16_t address);

public:
    memory_device(args_parser *args, json config);
    void clear();
    string get_source_line(uint16_t address);
    bool has_address(string label);
    uint16_t get_address(string label);
    string get_label(uint16_t address);
    void set_read_sequence(uint16_t address, vector<uint8_t> sequence, bool permanent);
    vector<uint8_t> get_write_sequence(uint16_t address, size_t length);
    uint8_t get_read_count(uint16_t address);
    uint8_t get_write_count(uint16_t address);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void print();
};
