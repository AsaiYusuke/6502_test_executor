#pragma once

#include "assert/message.hpp"
#include "condition/condition_register_a_x_y.hpp"
#include "condition/condition_register_a_x_y_value.hpp"
#include "emulation/cpu_device.hpp"
#include "emulation/emulation_devices.hpp"
#include "test/test_result.hpp"
#include "util/to_string.hpp"

using namespace std;

class assert_register_value
{
public:
    static bool test(emulation_devices *device, condition_register_a_x_y *register_def, test_result *result)
    {
        auto actual = device->get_cpu()->get_register8(register_def->get_type());

        auto expression = register_def->get_value()->get_expression();
        if (!expression)
            return true;

        bool total_result = expression->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_data(
                    "Value",
                    register_def,
                    to_string(*expression),
                    to_string(actual)));

        return total_result;
    }
};
