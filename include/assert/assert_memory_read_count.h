#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_memory_count.h"
#include "test_result.h"

using namespace std;

class assert_memory_read_count
{
public:
    static bool test(emulation_devices *device, condition_memory_count memory_count_def, test_result *result);
};
