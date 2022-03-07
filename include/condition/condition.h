#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_a_x_y.h"
#include "condition/condition_register_status_flag.h"
#include "condition/condition_register_pc.h"
#include "condition/condition_memory.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition
{
private:
    emulation_devices *device;
    vector<condition_register_a_x_y> register_defs;
    vector<condition_register_status_flag> status_flag_defs;
    vector<condition_memory> memory_defs;
    condition_register_pc *register_pc_def;
    bool timeout_def;

protected:
    emulation_devices *get_device();
    condition_register_pc *get_register_pc_def();
    vector<condition_register_a_x_y> get_register_defs();
    vector<condition_register_status_flag> get_status_flag_defs();
    vector<condition_memory> get_memory_defs();
    bool get_timeout_def();

public:
    condition(emulation_devices *_device, json condition_json, json target);
    condition(emulation_devices *_device, json condition_json);

    virtual void execute() = 0;
};
