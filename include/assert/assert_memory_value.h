#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_value.h"
#include "test_result.h"

using namespace std;

class assert_memory_value
{
public:
    static bool test(emulation_devices *device, condition_memory_value memory_value_def, test_result *result);
};
