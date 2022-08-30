#pragma once

#include "condition/condition.h"

using namespace std;

class emulation_devices;

class test_setup : condition
{
public:
    test_setup(emulation_devices *device, json definitions_def, json condition_json, json target);
    void execute();
};
