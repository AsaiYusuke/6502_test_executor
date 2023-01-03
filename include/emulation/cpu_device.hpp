#pragma once

#include <vector>

#include "nlohmann/json.hpp"
#include "enum/register_type.hpp"
#include "enum/interrupt_type.hpp"
#include "enum/runtime_error_type.hpp"
#include "enum/status_flag_type.hpp"

using namespace std;

using json = nlohmann::json;

class i_cpu_filter;
class condition_register_pc;
class condition_mocked_proc;
class args_parser;
class exec_mos6502;
class debug_info;
class emulation_devices;
class call_stack_filter;
class register_counter_filter;

class cpu_device
{
private:
    emulation_devices *device;

    exec_mos6502 *cpu;
    uint64_t max_cycle_count;
    uint64_t cycle_count;
    call_stack_filter *call_stack;
    register_counter_filter *register_counter;
    vector<i_cpu_filter *> filters;
    uint16_t endPC;
    map<uint16_t, interrupt_type> interrupt_defs;
    map<uint16_t, condition_mocked_proc *> mocked_proc_defs;

public:
    cpu_device(emulation_devices *_device, args_parser *args, json config, debug_info *debug);
    void clear(condition_register_pc *pc, vector<uint8_t> stack);
    void execute();
    void add_error_result(runtime_error_type type, string message);
    uint64_t get_max_cycle_count();
    uint64_t get_cycle_count();
    uint8_t get_register8(register_type type);
    uint16_t get_register16(register_type type);
    void set_register8(register_type type, uint8_t value);
    uint8_t get_read_count(register_type type);
    uint8_t get_write_count(register_type type);
    uint8_t get_read_count(status_flag_type type);
    uint8_t get_write_count(status_flag_type type);
    bool is_illegal_instruction();
    bool is_call_instruction();
    bool is_return_instruction();
    bool is_read_register_instruction(register_type type);
    bool is_write_register_instruction(register_type type);
    bool is_read_status_instruction(status_flag_type type);
    bool is_write_status_instruction(status_flag_type type);
    bool is_previous_returned_instruction();
    bool is_interrupt_instruction();
    bool is_mocked_proc_instruction();
    vector<uint8_t> get_stack();
    void add_interrupt_hook(interrupt_type type, uint16_t address);
    void add_mocked_proc_hook(condition_mocked_proc *mocked_proc_def);
    void execute_interrupt();
    void execute_mocked_proc();
    void execute_standard_instruction();
    void print();
    vector<uint16_t> get_call_stack();
};
