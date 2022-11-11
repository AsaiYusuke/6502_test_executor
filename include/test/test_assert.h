#pragma once

#include "condition/condition.h"
#include "test/test_result.h"

using namespace std;

class test_assert : condition
{
private:
    test_result result;

public:
    test_assert(emulation_devices *device, json condition_json);
    void execute();
    test_result get_result(string id);
};
