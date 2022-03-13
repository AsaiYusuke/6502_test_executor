#pragma once

#include "enum/operator_type.h"
#include "emulation/emulation_devices.h"

using namespace std;

class condition_memory_count
{
private:
    string name;
    uint16_t address;
    uint8_t count;
    vector<pair<operator_type, uint8_t>> expressions;

public:
    condition_memory_count(uint16_t address, json condition, string name);
    uint16_t get_address();
    uint8_t get_count();
    string get_name();
    vector<pair<operator_type, uint8_t>> get_expressions();
};
