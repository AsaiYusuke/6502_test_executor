#pragma once

#include "emulation/emulation_devices.h"

using namespace std;

class condition_memory_count
{
private:
    uint16_t address;
    uint8_t count;
    string name;

public:
    condition_memory_count(uint16_t address, uint8_t count, string name);
    uint16_t get_address();
    uint8_t get_count();
    string get_name();
};
