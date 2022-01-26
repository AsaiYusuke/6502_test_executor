#pragma once

#include "emulation_devices.h"
#include "condition/condition_register.h"
#include "condition/condition_status_register.h"
#include "condition/condition_pc_register.h"
#include "condition/condition_memory.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition
{
private:
    emulation_devices *device;
    vector<condition_register> registers;
    condition_status_register *register_status;
    condition_memory *memory;
    condition_pc_register *register_pc;

public:
    condition(emulation_devices *_device, json condition, json target);
    condition(emulation_devices *_device, json condition);
    emulation_devices *get_device();
    vector<condition_register> get_register_defs();
    vector<tuple<status_flag_type, bool, string>> get_status_flag_defs();
    vector<tuple<uint16_t, vector<uint8_t>, string>> get_memory_value_defs();
    vector<tuple<uint16_t, uint8_t, string>> get_memory_read_count_defs();
    vector<tuple<uint16_t, uint8_t, string>> get_memory_write_count_defs();
    condition_pc_register *get_pc_register_def();
};
