#pragma once

#include "assert/message.hpp"
#include "condition/condition_stack.hpp"
#include "emulation/emulation_devices.hpp"
#include "test/test_result.hpp"
#include "util/to_string.hpp"

using namespace std;

class assert_stack_value
{
public:
    static bool test(emulation_devices *device, condition_stack *stack_def, test_result *result)
    {
        auto expression_stack = stack_def->get_expressions();
        if (expression_stack.size() == 0)
            return true;

        bool total_result = true;
        auto actuals = device->get_cpu()->get_stack();
        for (decltype(expression_stack.size()) offset = 0, size = expression_stack.size(); offset < size && total_result; offset++)
            if (actuals.size() <= offset)
                total_result = false;
            else
                total_result &= expression_stack.at(offset).test(actuals.at(offset));

        if (!total_result)
            result->add_error(
                message::error_stack_data(
                    stack_def,
                    to_string(expression_stack),
                    to_string(actuals)));

        return total_result;
    }
};
