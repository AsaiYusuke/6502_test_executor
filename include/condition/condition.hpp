#pragma once

#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;
class condition_register_pc;
class condition_register_a_x_y;
class condition_register_status_flag;
class condition_memory;
class condition_stack;
class condition_interrupt;
class condition_mocked_proc;
class condition_timeout;

class condition
{
private:
    emulation_devices *device;
    condition_register_pc *register_pc_def;
    vector<condition_register_a_x_y *> register_defs;
    vector<condition_register_status_flag *> status_flag_defs;
    vector<condition_memory *> memory_defs;
    condition_stack *stack_def;
    vector<condition_interrupt *> interrupt_defs;
    vector<condition_mocked_proc *> mocked_proc_defs;
    condition_timeout *timeout_def;

protected:
    emulation_devices *get_device() const;
    condition_register_pc *get_register_pc_def() const;
    vector<condition_register_a_x_y *> get_register_defs() const;
    vector<condition_register_status_flag *> get_status_flag_defs() const;
    vector<condition_memory *> get_memory_defs() const;
    condition_stack *get_stack_def() const;
    vector<condition_interrupt *> get_interrupt_defs() const;
    vector<condition_mocked_proc *> get_mocked_proc_defs() const;
    condition_timeout *get_timeout_def() const;

public:
    condition(emulation_devices *_device, json condition_json, json target);
    condition(emulation_devices *_device, json condition_json);
};
