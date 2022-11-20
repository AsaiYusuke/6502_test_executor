#pragma once

#include <map>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "args_parser.h"
#include "memory_access.h"
#include "debug_info.h"
#include "emulation/rom_image.h"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class memory_device : public i_memory_access
{
private:
    emulation_devices *device;

    debug_info *debug;
    rom_image *rom;

    map<uint16_t, uint8_t> ram;

    map<uint16_t, vector<uint8_t>> read_sequences;
    map<uint16_t, bool> read_permanent;
    map<uint16_t, vector<uint8_t>> write_sequences;

    map<uint16_t, uint8_t> read_counts;
    map<uint16_t, uint8_t> write_counts;

    bool assert_invalid_memory;

public:
    memory_device(emulation_devices *_device, args_parser *args, json config);
    vector<int> get_detected_remove_segment_ids();
    void clear();
    string get_source_line(uint16_t address);
    uint16_t get_address(string label);
    void set_read_sequence(uint16_t address, vector<uint8_t> sequence, bool permanent);
    vector<uint8_t> get_write_sequence(uint16_t address, size_t length);
    uint8_t get_read_count(uint16_t address);
    uint8_t get_write_count(uint16_t address);
    uint8_t read(uint16_t address);
    uint8_t read_raw(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void write_raw(uint16_t address, uint8_t value);
    void print();
};
