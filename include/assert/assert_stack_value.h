#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_stack.h"
#include "test_result.h"
#include "message.h"
#include "util/to_string.h"
#include "util/expression_executer.h"

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
        for (decltype(expression_stack.size()) offset = 0, size = expression_stack.size(); offset < size; offset++)
        {
            bool success = true;
            if (actuals.size() <= offset)
                success = false;
            else
            {
                auto actual = actuals.at(offset);
                for (auto expression : expression_stack.at(offset))
                    if (!expression_executer::test(expression.first, actual, expression.second))
                    {
                        success = false;
                        break;
                    }
            }
            if (!success)
                result->add_error(
                    message::error_stack_data(
                        stack_def,
                        offset,
                        to_string(expression_stack),
                        to_string(actuals)));
            total_result &= success;
        }
        return total_result;
    }
};
