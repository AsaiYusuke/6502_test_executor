#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_status_flag.h"
#include "test/test_result.h"
#include "assert/message.h"
#include "util/to_string.h"

using namespace std;

class assert_status_flag_value
{
public:
    static bool test(emulation_devices *device, condition_register_status_flag status_flag_def, test_result *result)
    {
        uint8_t status = device->get_cpu()->get_register8(register_type::P);
        auto actual = (status & (uint8_t)status_flag_def.get_type()) > 0;

        auto expression = status_flag_def.get_value()->get_expression();
        if (!expression)
            return true;

        bool total_result = expression->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_status_flag_data(
                    "Value",
                    status_flag_def,
                    to_string(*expression),
                    to_string(actual)));

        return total_result;
    }
};
