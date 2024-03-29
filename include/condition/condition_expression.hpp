#pragma once

#include "enum/operator_type.hpp"
#include "util/expression_execute.hpp"

using namespace std;

class emulation_devices;

template <typename T, typename U>
class condition_expression
{
private:
    vector<pair<operator_type, U>> and_expressions;
    vector<condition_expression<T, U>> or_expressions;

public:
    condition_expression(const emulation_devices *device, json condition)
    {
        for (auto &expression : expression_execute::get(condition))
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

    bool test(const U actual) const
    {
        bool total_result = true;
        for (auto &expression : and_expressions)
            total_result &= expression_execute::test(expression.first, actual, expression.second);

        if (or_expressions.size() > 0)
        {
            bool or_result = false;
            for (auto &expression : or_expressions)
                or_result |= expression.test(actual);
            total_result &= or_result;
        }

        return total_result;
    }

    vector<pair<operator_type, U>> get_and_expressions() const
    {
        return and_expressions;
    }

    vector<condition_expression<T, U>> get_or_expressions() const
    {
        return or_expressions;
    }
};
