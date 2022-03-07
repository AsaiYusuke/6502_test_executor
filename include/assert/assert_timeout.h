#pragma once

#include "emulation/emulation_devices.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"

using namespace std;

template <typename T>
class assert_timeout
{
public:
    static bool test(emulation_devices *device, bool expected, test_result *result)
    {
        auto error_defs = device->get_filtered_errors({runtime_error_type::TIMEOUT});
        auto actual = !error_defs.empty();

        if (T::test(actual, expected))
            return true;

        result->add_error(
            message::trace_timeout(
                device,
                actual ? error_defs.front().get_call_stack() : vector<uint16_t>{},
                to_string(expected),
                to_string(actual)));

        return false;
    }
};
