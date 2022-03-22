#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_count.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_memory_write_count
{
public:
    static bool test(emulation_devices *device, condition_memory_count memory_count_def, test_result *result)
    {
        auto actual = device->get_memory()->get_write_count(
            memory_count_def.get_address());
        bool total_result = memory_count_def.get_expression()->test(actual);
        if (!total_result)
            result->add_error(
                message::error_memory_write_count(
                    memory_count_def,
                    to_string(memory_count_def.get_expression()),
                    to_string(actual)));

        return total_result;
    }
};
