#pragma once

#include <vector>

#include "mos6502.h"
#include "nlohmann/json.hpp"
#include "args_parser.h"
#include "emulation/cpu_filter/cpu_filter.h"
#include "emulation/cpu_filter/call_stack_filter.h"
#include "condition/condition_mocked_proc.h"
#include "condition/condition_mocked_value.h"
#include "enum/register_type.h"
#include "enum/interrupt_type.h"
#include "runtime_error_result.h"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class call_stack_filter;

class cpu_device
{
private:
    emulation_devices *device;

    mos6502 *cpu;
    uint64_t max_cycle_count;
    uint64_t cycle_count;
    enum class inst_type
    {
        call,
        retern
    };
    call_stack_filter *call_stack;
    vector<i_cpu_filter *> filters;
    uint16_t endPC;
    map<uint16_t, interrupt_type> interrupt_defs;
    map<uint16_t, condition_mocked_proc> mocked_proc_defs;

public:
    cpu_device(emulation_devices *_device, args_parser *args, json config);
    void clear(uint16_t startPC, uint16_t endPC, vector<uint8_t> stack);
    void execute();
    void add_error_result(runtime_error_type type);
    uint64_t get_max_cycle_count();
    uint64_t get_cycle_count();
    uint8_t get_register(register_type type);
    void set_register(register_type type, uint8_t value);
    bool is_call_instrunction();
    bool is_return_instruction();
    bool is_previous_returned_instruction();
    bool is_interrupt_instruction();
    bool is_mocked_proc_instruction();
    vector<uint8_t> get_stack();
    void add_interrupt_hook(interrupt_type type, uint16_t address);
    void add_mocked_proc_hook(condition_mocked_proc mocked_proc_def);
    void execute_interrupt();
    void execute_mocked_proc();
    void execute_standard_instruction();
    void print();
    vector<uint16_t> get_call_stack();
};
