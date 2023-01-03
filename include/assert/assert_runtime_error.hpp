#pragma once

#include "assert/runtime_error_result.hpp"
#include "test/test_result.hpp"
#include "assert/message.hpp"

using namespace std;

class emulation_devices;

class assert_runtime_error
{
public:
    static bool test(emulation_devices *device, runtime_error_result error_def, test_result *result)
    {
        switch (error_def.get_type())
        {
        case runtime_error_type::READONLY_MEMORY:
            result->add_error(
                message::trace_readonly_memory(
                    device,
                    error_def.get_call_stack(),
                    error_def.get_message()));
            break;
        case runtime_error_type::OUT_OF_SEGMENT:
            result->add_error(
                message::trace_out_of_segment(
                    device,
                    error_def.get_call_stack(),
                    error_def.get_message()));
            break;
        case runtime_error_type::ILLEGAL_INSTRUCTION:
            result->add_error(
                message::trace_illegal_instruction(
                    device,
                    error_def.get_call_stack(),
                    error_def.get_message()));
            break;
        }

        return false;
    }
};
