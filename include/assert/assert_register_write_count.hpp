#pragma once

#include "emulation/emulation_devices.hpp"
#include "condition/condition_register_a_x_y.hpp"
#include "condition/condition_register_a_x_y_count.hpp"
#include "test/test_result.hpp"
#include "assert/message.hpp"
#include "util/to_string.hpp"

using namespace std;

class assert_register_write_count
{
public:
    static bool test(emulation_devices *device, condition_register_a_x_y *register_def, test_result *result)
    {
        auto actual = device->get_cpu()->get_write_count(register_def->get_type());

        auto expression = register_def->get_write_count()->get_expression();
        if (!expression)
            return true;

        bool total_result = expression->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_data(
                    "Write count",
                    register_def,
                    to_string(*expression),
                    to_string(actual)));

        return total_result;
    }
};
