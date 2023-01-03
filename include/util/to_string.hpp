#pragma once

#include <string>
#include <vector>

#include "condition/condition_expression.hpp"
#include "enum/operator_type.hpp"

using namespace std;

string to_string(uint8_t value);
string to_string(bool value);
string to_string(operator_type value);

template <typename T>
string to_string(vector<T> values)
{
    stringstream ss;
    ss << "[";

    if (!values.empty())
    {
        ss << to_string(values[0]);
        for (decltype(values.size()) index = 1, size = values.size(); index < size; index++)
            ss << ", " << to_string(values[index]);
    }

    ss << "]";
    return ss.str();
}

template <typename T, typename U>
string to_string(pair<T, U> value)
{
    stringstream ss;
    ss << to_string(value.first) << ":" << to_string(value.second);
    return ss.str();
}

template <typename T, typename U>
string to_string(condition_expression<T, U> value)
{
    stringstream ss;
    if (value.get_and_expressions().size() > 0)
        ss << to_string(value.get_and_expressions());
    if (value.get_or_expressions().size() > 0)
    {
        if (value.get_and_expressions().size() > 0)
            ss << ", ";
        ss << "anyOf:";
        ss << to_string(value.get_or_expressions());
    }
    return ss.str();
}
