#pragma once

#include <stdint.h>
#include "emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_memory
{
private:
    vector<tuple<uint16_t, uint8_t, string>> memory_defs;
    string get_address_name(emulation_devices *device, json memory_def, int offset);
    vector<uint8_t> get_values(emulation_devices *_device, json memory_def);

public:
    condition_memory(emulation_devices *_device, json condition);
    vector<tuple<uint16_t, uint8_t, string>> get_memory_defs();
};
