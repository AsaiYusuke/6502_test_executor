#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_value.h"
#include "condition/condition_memory_count.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_memory
{
private:
    vector<condition_memory_value> value_sequences;
    vector<condition_memory_count> read_counts;
    vector<condition_memory_count> write_counts;

    string create_address_name(emulation_devices *device, json memory_def, int offset);
    json normalize_value_sequences(emulation_devices *_device, json memory_def);
    json normalize_read_counts(emulation_devices *_device, json memory_def);
    json normalize_write_counts(emulation_devices *_device, json memory_def);

public:
    condition_memory(emulation_devices *_device, json condition);
    vector<condition_memory_value> get_value_sequences();
    vector<condition_memory_count> get_read_counts();
    vector<condition_memory_count> get_write_counts();
};
