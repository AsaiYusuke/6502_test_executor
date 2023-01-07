#pragma once

#include "condition/condition.hpp"
#include "test/test_result.hpp"

using namespace std;

class test_assert : condition
{
private:
    test_result result;

public:
    test_assert(string id, emulation_devices *device, json condition_json);
    void execute();
    test_result get_result() const;
};
