#include "assert/assert_status_flag_value.h"
#include "message.h"
#include "util/to_string.h"

bool assert_status_flag_value::test(emulation_devices *device, condition_register_status_flag status_flag_def, test_result *result)
{
    uint8_t status = device->get_cpu()->get_register(register_type::P);
    auto expected = status_flag_def.get_value();
    auto actual = (status & (uint8_t)status_flag_def.get_type()) > 0;

    if (expected != actual)
    {
        result->add_error(
            message::error_register_status_flag_data(
                status_flag_def,
                to_string(expected),
                to_string(actual)));

        return false;
    }

    return true;
}
