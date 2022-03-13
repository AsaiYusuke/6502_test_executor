#pragma once

#include "enum/operator_type.h"
#include "emulation/emulation_devices.h"

using namespace std;

class condition_memory_value
{
private:
    string name;
    uint16_t address;
    vector<uint8_t> sequence;
    bool permanent;
    vector<vector<pair<operator_type, uint8_t>>> expression_sequence;

public:
    condition_memory_value(emulation_devices *device, uint16_t _address, json condition, bool _permanent, string _name);
    bool is_permanent();
    uint16_t get_address();
    vector<uint8_t> get_sequence();
    string get_name();
    vector<vector<pair<operator_type, uint8_t>>> get_expressions();
};
