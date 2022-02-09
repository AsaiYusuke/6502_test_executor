#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_status_flag.h"

using namespace std;

class assert_status_flag_value
{
public:
    static bool test(emulation_devices *device, condition_status_flag status_flag_def, vector<string> &errors);
};
