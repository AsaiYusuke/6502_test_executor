#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_value.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

using namespace std;

class assert_memory_value
{
public:
    static bool test(emulation_devices *device, condition_memory_value memory_value_def, test_result *result)
    {
        bool total_result = true;
        auto expression_sequences = memory_value_def.get_expressions();
        auto actuals = device->get_memory()->get_write_sequence(
            memory_value_def.get_address(),
            expression_sequences.size());
        for (decltype(expression_sequences.size()) offset = 0, size = expression_sequences.size(); offset < size; offset++)
        {
            auto actual = actuals.at(offset);
            bool success = true;
            for (auto expression : expression_sequences.at(offset))
                if (!expression_executer::test(expression.first, actual, expression.second))
                {
                    success = false;
                    break;
                }
            if (!success)
                result->add_error(
                    message::error_memory_data(
                        memory_value_def,
                        offset,
                        to_string(expression_sequences),
                        to_string(actuals)));
            total_result &= success;
        }
        return total_result;
    }
};
