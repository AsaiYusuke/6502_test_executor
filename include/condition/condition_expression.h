#pragma once

#include "enum/operator_type.h"
#include "emulation/emulation_devices.h"
#include "util/expression_executer.h"

using namespace std;

template <typename T, typename U>
class condition_expression
{
private:
    operator_type type;
    vector<pair<operator_type, U>> and_expressions;
    vector<condition_expression<T, U>> or_expressions;

public:
    condition_expression(emulation_devices *device, json condition)
    {
        for (auto &expression : expression_executer::get(condition))
        {
            if (expression.first == operator_type::ANY_OF)
            {
                for (auto &or_exp : expression.second)
                    or_expressions.push_back(
                        condition_expression<T, U>(device, or_exp));
                continue;
            }

            and_expressions.push_back(
                make_pair(
                    expression.first,
                    T::template convert<U>(device, expression.second)));
        }
    }

    bool test(U actual)
    {
        bool total_result = true;
        for (auto &expression : and_expressions)
            total_result &= expression_executer::test(expression.first, actual, expression.second);

        if (or_expressions.size() > 0)
        {
            bool or_result = false;
            for (auto &expression : or_expressions)
                or_result |= expression.test(actual);
            total_result &= or_result;
        }

        return total_result;
    }

    vector<pair<operator_type, U>> get_and_expressions()
    {
        return and_expressions;
    }

    vector<condition_expression<T, U>> get_or_expressions()
    {
        return or_expressions;
    }
};
