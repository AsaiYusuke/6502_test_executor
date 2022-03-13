#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_count.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_memory_read_count
{
public:
    static bool test(emulation_devices *device, condition_memory_count memory_count_def, test_result *result)
    {
        bool total_result = true;
        for (auto expression : memory_count_def.get_expressions())
        {
            auto actual = device->get_memory()->get_read_count(
                memory_count_def.get_address());

            if (expression_executer::test(expression.first, actual, expression.second))
                continue;

            result->add_error(
                message::error_memory_read_count(
                    memory_count_def,
                    to_string(expression.second),
                    to_string(actual)));

            total_result = false;
        }
        return total_result;
    }
};
