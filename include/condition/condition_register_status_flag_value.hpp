#pragma once

#include "condition/condition_expression.hpp"
#include "condition/expression_value.hpp"

using namespace std;

class condition_register_status_flag_value
{
private:
    bool value;
    condition_expression<expression_value, bool> *expression;

public:
    condition_register_status_flag_value(emulation_devices *device, json condition);
    bool get_value();
    condition_expression<expression_value, bool> *get_expression();
};
