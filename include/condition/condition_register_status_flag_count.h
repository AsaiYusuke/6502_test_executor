#pragma once

#include "condition/condition_expression.h"
#include "condition/expression_value.h"

using namespace std;

class condition_register_status_flag_count
{
private:
    condition_expression<expression_value, uint8_t> *expression;

public:
    condition_register_status_flag_count(emulation_devices *device, json condition);
    condition_expression<expression_value, uint8_t> *get_expression();
};