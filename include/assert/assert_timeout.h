#pragma once

#include "emulation/emulation_devices.h"

using namespace std;

class assert_timeout
{
public:
    static bool test(emulation_devices *device, bool expected, vector<string> &errors);
};
