#pragma once

#include "condition/message_name.hpp"
#include "condition/condition_expression.hpp"
#include "condition/expression_value.hpp"

using namespace std;

class emulation_devices;

class condition_memory_count : public i_message_name
{
private:
    uint16_t address;
    string name;
    condition_expression<expression_value, uint8_t> *expression;

public:
    condition_memory_count(emulation_devices *device, uint16_t address, json condition, string name);
    uint16_t get_address();
    string get_name();
    condition_expression<expression_value, uint8_t> *get_expression();
};
