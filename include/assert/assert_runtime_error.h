#pragma once

#include "emulation/emulation_devices.h"
#include "test_result.h"
#include "enum/operator_type.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_runtime_error
{
public:
    static bool test(emulation_devices *device, runtime_error_result error_def, test_result *result)
    {
        auto expected = false;
        auto actual = true;

        if (expression_executer::test(operator_type::EQ, actual, expected))
            return true;

        switch (error_def.get_type())
        {
        case runtime_error_type::READONLY_MEMORY:
            result->add_error(
                message::trace_readonly_memory(
                    device,
                    error_def.get_call_stack(),
                    error_def.get_message(),
                    to_string(expected),
                    to_string(actual)));
            break;
        case runtime_error_type::OUT_OF_SEGMENT:
            result->add_error(
                message::trace_out_of_segment(
                    device,
                    error_def.get_call_stack(),
                    error_def.get_message(),
                    to_string(expected),
                    to_string(actual)));
            break;
        }

        return false;
    }
};
