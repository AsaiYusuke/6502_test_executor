#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_value.h"

using namespace std;

class assert_memory_value
{
public:
    static bool test(emulation_devices *device, condition_memory_value memory_value_def, vector<string> &errors);
};
