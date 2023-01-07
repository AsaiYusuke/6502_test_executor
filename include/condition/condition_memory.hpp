#pragma once

#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;
class condition_memory_value;
class condition_memory_count;

class condition_memory
{
private:
    vector<condition_memory_value *> value_sequences;
    vector<condition_memory_count *> read_counts;
    vector<condition_memory_count *> write_counts;

    string create_address_name(const emulation_devices *device, json memory_def, int offset) const;
    json normalize_value_sequences(const emulation_devices *device, json memory_def) const;
    json normalize_read_counts(const emulation_devices *device, json memory_def) const;
    json normalize_write_counts(const emulation_devices *device, json memory_def) const;

public:
    condition_memory(const emulation_devices *device, json condition);
    vector<condition_memory_value *> get_value_sequences() const;
    vector<condition_memory_count *> get_read_counts() const;
    vector<condition_memory_count *> get_write_counts() const;
};
