#include "assert/assert_timeout.h"
#include "assert/operation/assert_equal.h"
#include "message.h"
#include "util/to_string.h"

bool assert_timeout::test(emulation_devices *device, bool expected, vector<string> &errors)
{
    auto error_defs = device->get_filtered_errors({runtime_error_type::TIMEOUT});
    auto actual = !error_defs.empty();

    if (expected != actual)
    {
        errors.push_back(
            message::trace_timeout(
                device,
                actual ? error_defs.front().get_call_stack() : vector<uint16_t>{},
                to_string(expected),
                to_string(actual)));

        return false;
    }

    return true;
}
