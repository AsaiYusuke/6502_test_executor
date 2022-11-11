#pragma once

#include "condition/message_name.h"
#include "condition/condition_register_a_x_y.h"
#include "condition/condition_register_status_flag.h"
#include "condition/condition_memory_value.h"
#include "condition/condition_memory_count.h"
#include "condition/condition_stack.h"

using namespace std;

class emulation_devices;

class message
{
public:
    static string error_message(string message, string expected, string actual);
    static string trace_message(emulation_devices *device, vector<uint16_t> call_stack, string message, string expected, string actual);

    static string error_register_data(string type, i_message_name &register_def, string expected, string actual);
    static string error_register_status_flag_data(string type, i_message_name &status_flag_def, string expected, string actual);
    static string error_memory_data(string type, i_message_name &memory_value_def, string expected, string actual);
    static string error_stack_data(condition_stack *stack_def, string expected, string actual);
    static string trace_timeout(emulation_devices *device, vector<uint16_t> call_stack, string expected, string actual);
    static string trace_readonly_memory(emulation_devices *device, vector<uint16_t> call_stack, string detail, string expected, string actual);
    static string trace_out_of_segment(emulation_devices *device, vector<uint16_t> call_stack, string detail, string expected, string actual);
};
