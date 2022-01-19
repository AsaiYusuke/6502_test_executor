#pragma once

#include <stdint.h>
#include "condition/address_convert.h"
#include "../../../../json/single_include/nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_memory : public address_convert
{
private:
    vector<tuple<uint16_t, uint8_t, string>> memory_defs;
    string to_hex_string(uint16_t value);
    string get_address_name(emulation_devices *device, json memory_def);

public:
    condition_memory(emulation_devices *_device, json condition);
    vector<tuple<uint16_t, uint8_t, string>> get_memory_defs();
};
