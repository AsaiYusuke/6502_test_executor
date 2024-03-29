#include "assert/message.hpp"

#include "condition/message_name.hpp"
#include "emulation/cpu_device.hpp"
#include "emulation/emulation_devices.hpp"
#include "emulation/memory_device.hpp"
#include "util/constant.hpp"
#include "util/to_string.hpp"
#include "util/value_convert.hpp"

string message::error_message(exception &e)
{
    stringstream ss;
    ss << e.what() << endl
       << endl;
    return ss.str();
}

string message::error_message(const string &message)
{
    stringstream ss;
    ss << message << endl;
    return ss.str();
}

string message::error_message(const string &message, const string &expected, const string &actual)
{
    stringstream ss;
    ss << message
       << ": Expected: " << expected
       << ", Actual: " << actual
       << endl;
    return ss.str();
}

string message::trace_message(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &message)
{
    stringstream ss;
    ss << error_message(message);
    ss << call_stack_message(device, call_stack);
    return ss.str();
}

string message::trace_message(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &message, const string &expected, const string &actual)
{
    stringstream ss;
    ss << error_message(message, expected, actual);
    ss << call_stack_message(device, call_stack);
    return ss.str();
}

string message::call_stack_message(const emulation_devices *device, const vector<uint16_t> &call_stack)
{
    if (call_stack.empty())
        return {};

    stringstream ss;

    ss << endl
       << "Call stack:" << endl;

    uint16_t prev_address = 0;
    int index = 0;
    for (auto address : call_stack)
    {
        if (address == DEFAULT_TEST_RETURN_ADDRESS)
            continue;

        if (prev_address == address)
            continue;

        prev_address = address;

        ss << "  " << index++ << ":  "
           << value_convert::to_zero_filled_hex_string(address) << " : "
           << device->get_memory()->get_source_line(address) << endl;
    }
    ss << endl;

    return ss.str();
}

string message::error_register_data(const string &type, const i_message_name *register_def, const string &expected, const string &actual)
{
    return error_message(
        "Register (" + type + ") [" + register_def->get_name() + "]",
        expected,
        actual);
}

string message::error_register_status_flag_data(const string &type, const i_message_name *status_flag_def, const string &expected, const string &actual)
{
    return error_message(
        "Register (" + type + ") [P (" + status_flag_def->get_name() + ")]",
        expected,
        actual);
}

string message::error_memory_data(const string &type, const i_message_name *memory_def, const string &expected, const string &actual)
{
    return error_message(
        "Memory (" + type + ") [" + memory_def->get_name() + "]",
        expected,
        actual);
}

string message::error_stack_data(const condition_stack *stack_def, const string &expected, const string &actual)
{
    return error_message(
        "Stack data",
        expected,
        actual);
}

string message::trace_timeout(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail, const string &expected, const string &actual)
{
    return trace_message(
        device,
        call_stack,
        "Timeout [" + detail + ", execution count >= " + to_string(device->get_cpu()->get_max_cycle_count()) + "]",
        expected,
        actual);
}

string message::trace_readonly_memory(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail)
{
    return trace_message(
        device,
        call_stack,
        "Attempted write to readonly memory [" + detail + "]");
}

string message::trace_out_of_segment(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail)
{
    return trace_message(
        device,
        call_stack,
        "Out of segment [" + detail + "]");
}

string message::trace_illegal_instruction(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail)
{
    return trace_message(
        device,
        call_stack,
        "Illegal instruction [" + detail + "]");
}

string message::trace_unauthorized_memory(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail)
{
    return trace_message(
        device,
        call_stack,
        "Attempted access to unauthorized memory [" + detail + "]");
}

string message::trace_uninitialized_memory(const emulation_devices *device, const vector<uint16_t> &call_stack, const string &detail)
{
    return trace_message(
        device,
        call_stack,
        "Attempted read to uninitialized memory [" + detail + "]");
}
