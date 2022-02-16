#pragma once

#include "emulation/emulation_devices.h"

using namespace std;

class assert_runtime_error
{
public:
    static bool test(emulation_devices *device, runtime_error_result error_def, vector<string> &errors);
};
