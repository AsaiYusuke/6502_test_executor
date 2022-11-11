#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_a_x_y.h"
#include "test/test_result.h"
#include "assert/message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_register_read_count
{
public:
    static bool test(emulation_devices *device, condition_register_a_x_y register_def, test_result *result)
    {
        auto actual = device->get_cpu()->get_read_count(register_def.get_type());
        
        auto expression = register_def.get_read_count()->get_expression();
        if (!expression)
            return true;

        bool total_result = expression->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_data(
                    "Read count",
                    register_def,
                    to_string(*expression),
                    to_string(actual)));

        return total_result;
    }
};
