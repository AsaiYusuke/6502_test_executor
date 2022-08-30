#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_a_x_y.h"
#include "test/test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_register_value
{
public:
    static bool test(emulation_devices *device, condition_register_a_x_y register_def, test_result *result)
    {
        auto actual = device->get_cpu()->get_register(
            register_def.get_type());
        bool total_result = register_def.get_expression()->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_data(
                    register_def,
                    to_string(*register_def.get_expression()),
                    to_string(actual)));

        return total_result;
    }
};
