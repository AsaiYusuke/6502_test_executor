#pragma once

#include "emulation/emulation_devices.h"
#include "test_result.h"

using namespace std;

class assert_timeout
{
public:
    static bool test(emulation_devices *device, bool expected, test_result *result);
};
