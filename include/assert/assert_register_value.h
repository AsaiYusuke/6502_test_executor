#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_a_x_y.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_register_value
{
public:
    static bool test(emulation_devices *device, condition_register_a_x_y register_def, test_result *result)
    {
        bool total_result = true;
        auto actual = device->get_cpu()->get_register(
            register_def.get_type());
        for (auto expression : register_def.get_expressions())
        {
            if (expression_executer::test(expression.first, actual, expression.second))
                continue;

            result->add_error(
                message::error_register_data(
                    register_def,
                    to_string(expression.second),
                    to_string(actual)));

            total_result = false;
        }
        return total_result;
    }
};
