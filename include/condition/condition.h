#pragma once

#include "condition/condition_register_a_x_y.h"
#include "condition/condition_register_status_flag.h"
#include "condition/condition_register_pc.h"
#include "condition/condition_memory.h"
#include "condition/condition_stack.h"
#include "condition/condition_interrupt.h"
#include "condition/condition_mocked_proc.h"
#include "condition/condition_timeout.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition
{
private:
    emulation_devices *device;
    vector<condition_register_a_x_y> register_defs;
    vector<condition_register_status_flag> status_flag_defs;
    vector<condition_memory> memory_defs;
    condition_register_pc *register_pc_def;
    condition_stack *stack_def;
    vector<condition_interrupt> interrupt_defs;
    vector<condition_mocked_proc> mocked_proc_defs;
    condition_timeout *timeout_def;

protected:
    emulation_devices *get_device();
    condition_register_pc *get_register_pc_def();
    vector<condition_register_a_x_y> get_register_defs();
    vector<condition_register_status_flag> get_status_flag_defs();
    vector<condition_memory> get_memory_defs();
    condition_stack *get_stack_def();
    vector<condition_interrupt> get_interrupt_defs();
    vector<condition_mocked_proc> get_mocked_proc_defs();
    condition_timeout *get_timeout_def();

public:
    condition(emulation_devices *_device, json condition_json, json target);
    condition(emulation_devices *_device, json condition_json);
};
