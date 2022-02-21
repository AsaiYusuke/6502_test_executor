#pragma once

#include "emulation/emulation_devices.h"
#include "test_result.h"

using namespace std;

class assert_runtime_error
{
public:
    static bool test(emulation_devices *device, runtime_error_result error_def, test_result *result);
};
