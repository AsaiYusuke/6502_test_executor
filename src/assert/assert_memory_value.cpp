#include "assert/assert_memory_value.h"
#include "message.h"
#include "util/to_string.h"

bool assert_memory_value::test(emulation_devices *device, condition_memory_value memory_value_def, test_result *result)
{
    auto expected = memory_value_def.get_sequence();
    auto actual = device->get_memory()->get_write_sequence(
        memory_value_def.get_address(),
        expected.size());

    if (expected != actual)
    {
        result->add_error(
            message::error_memory_data(
                memory_value_def,
                to_string(expected),
                to_string(actual)));

        return false;
    }

    return true;
}
