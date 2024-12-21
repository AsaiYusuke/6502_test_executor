#pragma once

#include <cstdint>

#include "assert/message.hpp"
#include "assert/runtime_error_result.hpp"
#include "condition/condition_timeout.hpp"
#include "emulation/emulation_devices.hpp"
#include "test/test_result.hpp"
#include "util/to_string.hpp"

using namespace std;

class assert_timeout
{
public:
    static bool test(const emulation_devices *device, const condition_timeout *timeout_def, test_result *result)
    {
        auto error_defs = device->get_filtered_errors({runtime_error_type::TIMEOUT});
        auto actual = !error_defs.empty();
        bool total_result = timeout_def->get_expression()->test(actual);
        if (!total_result)
            result->add_error(
                message::trace_timeout(
                    device,
                    actual ? error_defs.front()->get_call_stack() : vector<uint16_t>{},
                    error_defs.front()->get_message(),
                    to_string(*timeout_def->get_expression()),
                    to_string(actual)));

        return total_result;
    }
};
