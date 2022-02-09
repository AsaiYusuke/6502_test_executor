#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_count.h"

using namespace std;

class assert_memory_write_count
{
public:
    static bool test(emulation_devices *device, condition_memory_count memory_count_def, vector<string> &errors);
};
