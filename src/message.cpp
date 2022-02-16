#include "message.h"
#include "util/to_string.h"
#include "util/address_convert.h"

string message::error_message(string message, string expected, string actual)
{
    stringstream ss;
    ss << message
       << ": Expected: " << expected
       << ", Actual: " << actual
       << endl;
    return ss.str();
}

string message::trace_message(emulation_devices *device, vector<uint16_t> call_stack, string message, string expected, string actual)
{
    stringstream ss;
    ss << error_message(message, expected, actual);

    if (!call_stack.empty())
    {
        ss << endl
           << "Call stack:" << endl;

        uint16_t prev_address = 0;
        int index = 0;
        for (auto address : call_stack)
        {
            if (address == 0xFFFF)
                continue;

            if (prev_address == address)
                continue;
            
            prev_address = address;

            ss << "  " << index++ << ":  "
               << address_convert::to_hex_string(address) << " : "
               << device->get_memory()->get_source_line(address) << endl;
        }
        ss << endl;
    }

    return ss.str();
}

string message::error_register_data(condition_register_a_x_y register_def, string expected, string actual)
{
    return error_message(
        "Register [" + register_def.get_name() + "]",
        expected,
        actual);
}

string message::error_register_status_flag_data(condition_register_status_flag status_flag_def, string expected, string actual)
{
    return error_message(
        "Register [P (" + status_flag_def.get_name() + ")]",
        expected,
        actual);
}

string message::error_memory_data(condition_memory_value memory_value_def, string expected, string actual)
{
    return error_message(
        "Memory data [" + memory_value_def.get_name() + "]",
        expected,
        actual);
}

string message::error_memory_read_count(condition_memory_count memory_count_def, string expected, string actual)
{
    return error_message(
        "Memory read count [" + memory_count_def.get_name() + "]",
        expected,
        actual);
}

string message::error_memory_write_count(condition_memory_count memory_count_def, string expected, string actual)
{
    return error_message(
        "Memory write count [" + memory_count_def.get_name() + "]",
        expected,
        actual);
}

string message::trace_timeout(emulation_devices *device, vector<uint16_t> call_stack, string expected, string actual)
{
    return trace_message(
        device,
        call_stack,
        "Timeout [execution count >= " + to_string(device->get_cpu()->get_timeout_threshold()) + "]",
        expected,
        actual);
}

string message::trace_readonly_memory(emulation_devices *device, vector<uint16_t> call_stack, string detail, string expected, string actual)
{
    return trace_message(
        device,
        call_stack,
        "Attempted write to readonly memory [" + detail + "]",
        expected,
        actual);
}

string message::trace_out_of_segment(emulation_devices *device, vector<uint16_t> call_stack, string detail, string expected, string actual)
{
    return trace_message(
        device,
        call_stack,
        "Out of segment [" + detail + "]",
        expected,
        actual);
}
