#pragma once

#include <string>
#include <vector>
#include "util/address_convert.h"
#include "util/to_string.h"

using namespace std;

class assert_equal
{
public:
    template <typename T>
    static bool test(T expected, T actual, string message, vector<string> &errors)
    {
        if (expected != actual)
        {
            errors.push_back(message + ": Expected: " + to_string(expected) + ", Actual: " + to_string(actual));
            return false;
        }
        return true;
    }
};
