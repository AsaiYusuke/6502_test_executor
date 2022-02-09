#pragma once

#include "emulation/emulation_devices.h"

using namespace std;

class condition_memory_value
{
private:
    uint16_t address;
    vector<uint8_t> sequence;
    string name;
    bool permanent;

public:
    condition_memory_value(uint16_t _address, vector<uint8_t> _sequence, bool _permanent, string _name);
    bool is_permanent();
    uint16_t get_address();
    vector<uint8_t> get_sequence();
    string get_name();
};
