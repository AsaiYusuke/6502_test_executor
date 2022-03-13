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
        bool total_result = true;
        auto error_defs = device->get_filtered_errors({runtime_error_type::TIMEOUT});
        auto actual = !error_defs.empty();
        for (auto expression : timeout_def->get_expressions())
        {
            if (expression_executer::test(expression.first, actual, expression.second))
                continue;

            result->add_error(
                message::trace_timeout(
                    device,
                    actual ? error_defs.front().get_call_stack() : vector<uint16_t>{},
                    to_string(expression.second),
                    to_string(actual)));

            total_result = false;
        }
        return total_result;
    }
};
