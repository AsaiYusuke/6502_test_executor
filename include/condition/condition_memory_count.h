#pragma once

#include "condition/condition_expression.h"
#include "condition/expression_value.h"

using namespace std;

class emulation_devices;

class condition_memory_count
{
private:
    string name;
    uint16_t address;
    uint8_t count;
    condition_expression<expression_value, uint8_t> *expression;

public:
    condition_memory_count(emulation_devices *device, uint16_t address, json condition, string name);
    uint16_t get_address();
    uint8_t get_count();
    string get_name();
    condition_expression<expression_value, uint8_t> *get_expression();
};
