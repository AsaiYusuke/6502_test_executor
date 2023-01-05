#pragma once

#include "nlohmann/json.hpp"

#include "enum/operator_type.hpp"

using namespace std;

using json = nlohmann::json;

class expression_execute
{
public:
    static bool find(json value);
    static vector<pair<operator_type, json>> get(json value);
    template <typename T>
    static bool test(operator_type type, T a, T b)
    {
        switch (type)
        {
        case operator_type::EQ:
            return a == b;
        case operator_type::GE:
            return a >= b;
        case operator_type::GT:
            return a > b;
        case operator_type::LE:
            return a <= b;
        case operator_type::LT:
            return a < b;
        case operator_type::NE:
            return a != b;
        }
        return false;
    }
};
