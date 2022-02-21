#include "assert/assert_register_value.h"
#include "message.h"
#include "util/to_string.h"

bool assert_register_value::test(emulation_devices *device, condition_register_a_x_y register_def, test_result *result)
{
    auto expected = register_def.get_value();
    auto actual = device->get_cpu()->get_register(
        register_def.get_type());

    if (expected != actual)
    {
        result->add_error(
            message::error_register_data(
                register_def,
                to_string(expected),
                to_string(actual)));

        return false;
    }

    return true;
}
