#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_a_x_y.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"

using namespace std;

template <typename T>
class assert_register_value
{
public:
    static bool test(emulation_devices *device, condition_register_a_x_y register_def, test_result *result)
    {
        auto expected = register_def.get_value();
        auto actual = device->get_cpu()->get_register(
            register_def.get_type());

        if (T::test(actual, expected))
            return true;

        result->add_error(
            message::error_register_data(
                register_def,
                to_string(expected),
                to_string(actual)));

        return false;
    }
};
