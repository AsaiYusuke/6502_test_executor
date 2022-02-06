#pragma once

#include "condition/condition.h"
#include "test_result.h"

using namespace std;

class test_assert : condition
{
private:
    test_result result;
    vector<string> errors;

    void err(string expected, string actual, string message);
    template <typename T>
    bool assert_equal(T expected, T actual, string message);

public:
    test_assert(emulation_devices *device, json condition_json);
    void execute();
    test_result get_result();
    vector<string> get_errors();
};
