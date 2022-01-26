#pragma once

#include <stdint.h>
#include "emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_memory
{
private:
    vector<tuple<uint16_t, vector<uint8_t>, string>> memory_value_defs;
    vector<tuple<uint16_t, uint8_t, string>> memory_read_count_defs;
    vector<tuple<uint16_t, uint8_t, string>> memory_write_count_defs;
    string get_address_name(emulation_devices *device, json memory_def, int offset);
    vector<vector<uint8_t>> get_value_sequences(emulation_devices *_device, json memory_def);
    vector<uint8_t> get_read_counts(emulation_devices *_device, json memory_def);
    vector<uint8_t> get_write_counts(emulation_devices *_device, json memory_def);

public:
    condition_memory(emulation_devices *_device, json condition);
    vector<tuple<uint16_t, vector<uint8_t>, string>> get_memory_value_defs();
    vector<tuple<uint16_t, uint8_t, string>> get_memory_read_count_defs();
    vector<tuple<uint16_t, uint8_t, string>> get_memory_write_count_defs();
};
