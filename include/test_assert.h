#pragma once

#include "condition/condition.h"
#include "test_result.h"

using namespace std;

class test_assert
{
private:
    condition *cond;
    test_result result;
    vector<string> errors;

    void err(string expected, string actual, string message);
    template <typename T>
    bool assert_equal(T expected, T actual, string message);

public:
    test_assert(condition *cond);
    void execute();
    test_result get_result();
    vector<string> get_errors();
};
