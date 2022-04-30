#pragma once

#include "condition/condition_expression.h"
#include "condition/expression_two_complement_byte.h"

using namespace std;

class emulation_devices;

class condition_memory_value
{
private:
    string name;
    uint16_t address;
    vector<uint8_t> sequence;
    bool permanent;
    vector<condition_expression<expression_two_complement_byte, uint8_t>> expression_sequence;;

public:
    condition_memory_value(emulation_devices *device, uint16_t _address, json condition, bool _permanent, string _name);
    bool is_permanent();
    uint16_t get_address();
    vector<uint8_t> get_sequence();
    string get_name();
    vector<condition_expression<expression_two_complement_byte, uint8_t>> get_expressions();
};
