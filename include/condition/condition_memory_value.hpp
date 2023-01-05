#pragma once

#include "condition/message_name.hpp"
#include "condition/condition_expression.hpp"
#include "condition/expression_two_complement_byte.hpp"

using namespace std;

class emulation_devices;

class condition_memory_value : public i_message_name
{
private:
    string name;
    uint16_t address;
    vector<uint8_t> sequence;
    bool permanent;
    vector<condition_expression<expression_two_complement_byte, uint8_t>> expression_sequence;

public:
    condition_memory_value(emulation_devices *device, uint16_t _address, json condition, bool _permanent, string _name);
    uint16_t get_address();
    string get_name();
    bool is_permanent();
    vector<uint8_t> get_sequence();
    vector<condition_expression<expression_two_complement_byte, uint8_t>> get_expressions();
};
