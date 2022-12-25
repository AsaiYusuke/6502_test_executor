#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_status_flag.h"
#include "test/test_result.h"
#include "assert/message.h"
#include "util/to_string.h"

using namespace std;

class assert_status_flag_read_count
{
public:
    static bool test(emulation_devices *device, condition_register_status_flag status_flag_def, test_result *result)
    {
        auto actual = device->get_cpu()->get_read_count(status_flag_def.get_type());

        auto expression = status_flag_def.get_read_count()->get_expression();
        if (!expression)
            return true;

        bool total_result = expression->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_status_flag_data(
                    "Read count",
                    status_flag_def,
                    to_string(*expression),
                    to_string(actual)));

        return total_result;
    }
};
