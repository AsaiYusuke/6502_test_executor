#include "assert/assert_memory_write_count.h"
#include "message.h"
#include "util/to_string.h"

bool assert_memory_write_count::test(emulation_devices *device, condition_memory_count memory_count_def, vector<string> &errors)
{
    auto expected = memory_count_def.get_count();
    auto actual = device->get_memory()->get_write_count(
        memory_count_def.get_address());

    if (expected != actual)
    {
        errors.push_back(
            message::error_memory_write_count(
                memory_count_def,
                to_string(expected),
                to_string(actual)));

        return false;
    }

    return true;
}
