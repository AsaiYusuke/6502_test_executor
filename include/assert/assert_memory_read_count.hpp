#pragma once

#include "emulation/emulation_devices.hpp"
#include "emulation/memory_device.hpp"
#include "condition/condition_memory_count.hpp"
#include "test/test_result.hpp"
#include "assert/message.hpp"
#include "util/to_string.hpp"

using namespace std;

class assert_memory_read_count
{
public:
    static bool test(emulation_devices *device, condition_memory_count *memory_count_def, test_result *result)
    {
        auto actual = device->get_memory()->get_read_count(
            memory_count_def->get_address());
        bool total_result = memory_count_def->get_expression()->test(actual);
        if (!total_result)
            result->add_error(
                message::error_memory_data(
                    "Read count",
                    memory_count_def,
                    to_string(*memory_count_def->get_expression()),
                    to_string(actual)));

        return total_result;
    }
};
