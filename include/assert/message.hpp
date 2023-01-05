#pragma once

#include "condition/condition_stack.hpp"
#include "condition/message_name.hpp"

using namespace std;

class emulation_devices;

class message
{
private:
    static string error_message(string message);
    static string error_message(string message, string expected, string actual);
    static string trace_message(emulation_devices *device, vector<uint16_t> call_stack, string message);
    static string trace_message(emulation_devices *device, vector<uint16_t> call_stack, string message, string expected, string actual);
    static string call_stack_message(emulation_devices *device, vector<uint16_t> call_stack);

public:
    static string error_message(exception &e);

    static string error_register_data(string type, i_message_name *register_def, string expected, string actual);
    static string error_register_status_flag_data(string type, i_message_name *status_flag_def, string expected, string actual);
    static string error_memory_data(string type, i_message_name *memory_value_def, string expected, string actual);
    static string error_stack_data(condition_stack *stack_def, string expected, string actual);
    static string trace_timeout(emulation_devices *device, vector<uint16_t> call_stack, string detail, string expected, string actual);
    static string trace_readonly_memory(emulation_devices *device, vector<uint16_t> call_stack, string detail);
    static string trace_out_of_segment(emulation_devices *device, vector<uint16_t> call_stack, string detail);
    static string trace_illegal_instruction(emulation_devices *device, vector<uint16_t> call_stack, string detail);
    static string trace_unauthorized_memory(emulation_devices *device, vector<uint16_t> call_stack, string detail);
    static string trace_uninitialized_memory(emulation_devices *device, vector<uint16_t> call_stack, string detail);
};
