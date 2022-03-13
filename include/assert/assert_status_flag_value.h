#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_status_flag.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_status_flag_value
{
public:
    static bool test(emulation_devices *device, condition_register_status_flag status_flag_def, test_result *result)
    {
        bool total_result = true;
        uint8_t status = device->get_cpu()->get_register(register_type::P);
        auto actual = (status & (uint8_t)status_flag_def.get_type()) > 0;
        for (auto expression : status_flag_def.get_expressions())
        {
            if (expression_executer::test(expression.first, actual, expression.second))
                continue;

            result->add_error(
                message::error_register_status_flag_data(
                    status_flag_def,
                    to_string(expression.second),
                    to_string(actual)));

            total_result = false;
        }
        return total_result;
    }
};
