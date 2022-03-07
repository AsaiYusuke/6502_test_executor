#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_value.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"

using namespace std;

template <typename T>
class assert_memory_value
{
public:
    static bool test(emulation_devices *device, condition_memory_value memory_value_def, test_result *result)
    {
        auto expected = memory_value_def.get_sequence();
        auto actual = device->get_memory()->get_write_sequence(
            memory_value_def.get_address(),
            expected.size());

        if (T::test(actual, expected))
            return true;

        result->add_error(
            message::error_memory_data(
                memory_value_def,
                to_string(expected),
                to_string(actual)));

        return false;
    }
};
