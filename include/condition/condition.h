#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register.h"
#include "condition/condition_status_flag.h"
#include "condition/condition_pc_register.h"
#include "condition/condition_memory.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition
{
private:
    emulation_devices *device;
    vector<condition_register> register_defs;
    vector<condition_status_flag> status_flag_defs;
    vector<condition_memory> memory_defs;
    condition_pc_register *register_pc;

protected:
    emulation_devices *get_device();
    condition_pc_register *get_pc_register_def();
    vector<condition_register> get_register_defs();
    vector<condition_status_flag> get_status_flag_defs();
    vector<condition_memory> get_memory_defs();

public:
    condition(emulation_devices *_device, json condition_json, json target);
    condition(emulation_devices *_device, json condition_json);

    virtual void execute() = 0;
};
