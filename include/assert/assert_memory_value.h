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
        auto expression_sequences = memory_value_def.get_expressions();
        if (expression_sequences.size() == 0)
            return true;

        bool total_result = true;
        auto actuals = device->get_memory()->get_write_sequence(
            memory_value_def.get_address(),
            expression_sequences.size());
        for (decltype(expression_sequences.size()) offset = 0, size = expression_sequences.size(); offset < size && total_result; offset++)
            if (actuals.size() <= offset)
                total_result = false;
            else
                total_result &= expression_sequences.at(offset).test(actuals.at(offset));

        if (!total_result)
            result->add_error(
                message::error_memory_data(
                    memory_value_def,
                    to_string(expression_sequences),
                    to_string(actuals)));

        return total_result;
    }
};
