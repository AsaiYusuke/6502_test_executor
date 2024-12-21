#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>

#include "nlohmann/json.hpp"

#include "memory_access.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;
class args_parser;
class debug_info;
class rom_image;

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

    bool assert_write_to_readonly_memory;
    bool assert_access_to_out_of_segment;
    bool assert_read_from_uninitialized_memory;
    bool assert_access_to_unauthorized_memory;

public:
    memory_device(emulation_devices *_device, const args_parser *args, json config, debug_info *debug);
    vector<int> get_detected_remove_segment_ids() const;
    void clear();
    string get_source_line(uint16_t address) const;
    uint16_t get_address(const string &label) const;
    void set_read_sequence(uint16_t address, vector<uint8_t> sequence, bool permanent);
    vector<uint8_t> get_write_sequence(uint16_t address, size_t length);
    uint8_t get_read_count(uint16_t address) const;
    uint8_t get_write_count(uint16_t address) const;
    uint8_t read(uint16_t address);
    uint8_t read_raw(uint16_t address);
    uint8_t read_data(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void write_raw(uint16_t address, uint8_t value);
    void write_data(uint16_t address, uint8_t value);
    void print() const;
};
