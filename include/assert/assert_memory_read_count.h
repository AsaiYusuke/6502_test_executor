#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_count.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"

using namespace std;

template <typename T>
class assert_memory_read_count
{
public:
    static bool test(emulation_devices *device, condition_memory_count memory_count_def, test_result *result)
    {
        auto expected = memory_count_def.get_count();
        auto actual = device->get_memory()->get_read_count(
            memory_count_def.get_address());

        if (T::test(actual, expected))
            return true;

        result->add_error(
            message::error_memory_read_count(
                memory_count_def,
                to_string(expected),
                to_string(actual)));

        return false;
    }
};
