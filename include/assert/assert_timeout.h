#pragma once

#include "emulation/emulation_devices.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_timeout
{
public:
    static bool test(emulation_devices *device, condition_timeout *timeout_def, test_result *result)
    {
        auto error_defs = device->get_filtered_errors({runtime_error_type::TIMEOUT});
        auto actual = !error_defs.empty();
        bool total_result = timeout_def->get_expression()->test(actual);
        if (!total_result)
            result->add_error(
                message::trace_timeout(
                    device,
                    actual ? error_defs.front().get_call_stack() : vector<uint16_t>{},
                    to_string(timeout_def->get_expression()),
                    to_string(actual)));

        return total_result;
    }
};
