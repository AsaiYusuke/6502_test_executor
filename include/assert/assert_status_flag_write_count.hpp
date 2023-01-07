#pragma once

#include "assert/message.hpp"
#include "condition/condition_register_status_flag.hpp"
#include "condition/condition_register_status_flag_count.hpp"
#include "emulation/emulation_devices.hpp"
#include "test/test_result.hpp"
#include "util/to_string.hpp"

using namespace std;

class assert_status_flag_write_count
{
public:
    static bool test(const emulation_devices *device, const condition_register_status_flag *status_flag_def, test_result *result)
    {
        auto actual = device->get_cpu()->get_write_count(status_flag_def->get_type());

        auto expression = status_flag_def->get_write_count()->get_expression();
        if (!expression)
            return true;

        bool total_result = expression->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_status_flag_data(
                    "Write count",
                    status_flag_def,
                    to_string(*expression),
                    to_string(actual)));

        return total_result;
    }
};
