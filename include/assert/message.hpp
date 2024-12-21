#pragma once

#include "condition/condition_stack.hpp"
#include "condition/message_name.hpp"

using namespace std;

class emulation_devices;

class message
{
private:
    static string error_message(const string &message);
    static string error_message(const string &message, const string &expected, const string &actual);
    static string trace_message(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &message);
    static string trace_message(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &message, const string &expected, const string &actual);
    static string call_stack_message(const emulation_devices *device, const vector<uint16_t> &call_stack);

public:
    static string error_message(exception &e);

    static string error_register_data(const string &type, const i_message_name *register_def, const string &expected, const string &actual);
    static string error_register_status_flag_data(const string &type, const i_message_name *status_flag_def, const string &expected, const string &actual);
    static string error_memory_data(const string &type, const i_message_name *memory_value_def, const string &expected, const string &actual);
    static string error_stack_data(const condition_stack *stack_def, const string &expected, const string &actual);
    static string trace_timeout(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail, const string &expected, const string &actual);
    static string trace_readonly_memory(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail);
    static string trace_out_of_segment(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail);
    static string trace_illegal_instruction(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail);
    static string trace_unauthorized_memory(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail);
    static string trace_uninitialized_memory(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail);
};
