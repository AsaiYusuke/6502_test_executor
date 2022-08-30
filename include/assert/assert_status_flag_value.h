#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_status_flag.h"
#include "test/test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_status_flag_value
{
public:
    static bool test(emulation_devices *device, condition_register_status_flag status_flag_def, test_result *result)
    {
        uint8_t status = device->get_cpu()->get_register(register_type::P);
        auto actual = (status & (uint8_t)status_flag_def.get_type()) > 0;
        bool total_result = status_flag_def.get_expression()->test(actual);
        if (!total_result)
            result->add_error(
                message::error_register_status_flag_data(
                    status_flag_def,
                    to_string(*status_flag_def.get_expression()),
                    to_string(actual)));

        return total_result;
    }
};
