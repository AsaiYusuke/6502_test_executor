#pragma once

#include "emulation/emulation_devices.h"
#include "condition/condition_register_a_x_y.h"

using namespace std;

class assert_register_value
{
public:
    static bool test(emulation_devices *device, condition_register_a_x_y register_def, vector<string> &errors);
};
