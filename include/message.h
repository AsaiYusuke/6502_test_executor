#pragma once

#include "condition/condition_register_a_x_y.h"
#include "condition/condition_register_status_flag.h"
#include "condition/condition_memory_value.h"
#include "condition/condition_memory_count.h"

using namespace std;

class message
{
public:
    static string error_message(string message, string expected, string actual);
    static string trace_message(emulation_devices *device, vector<uint16_t> call_stack, string message, string expected, string actual);

    static string error_register_data(condition_register_a_x_y register_def, string expected, string actual);
    static string error_register_status_flag_data(condition_register_status_flag status_flag_def, string expected, string actual);
    static string error_memory_data(condition_memory_value memory_value_def, int index, string expected, string actual);
    static string error_memory_read_count(condition_memory_count memory_count_def, string expected, string actual);
    static string error_memory_write_count(condition_memory_count memory_count_def, string expected, string actual);
    static string trace_timeout(emulation_devices *device, vector<uint16_t> call_stack, string expected, string actual);
    static string trace_readonly_memory(emulation_devices *device, vector<uint16_t> call_stack, string detail, string expected, string actual);
    static string trace_out_of_segment(emulation_devices *device, vector<uint16_t> call_stack, string detail, string expected, string actual);
};
