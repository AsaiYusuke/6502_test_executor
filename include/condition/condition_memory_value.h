#pragma once

#include "emulation/emulation_devices.h"

using namespace std;

class condition_memory_value
{
private:
    uint16_t address;
    vector<uint8_t> sequence;
    string name;

public:
    condition_memory_value(uint16_t address, vector<uint8_t> sequence, string name);
    uint16_t get_address();
    vector<uint8_t> get_sequence();
    string get_name();
};
