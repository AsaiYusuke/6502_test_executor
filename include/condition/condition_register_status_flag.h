#pragma once

#include "enum/status_flag_type.h"
#include "condition/condition_expression.h"
#include "condition/expression_value.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition_register_status_flag
{
private:
    string name;
    status_flag_type type;
    bool value;
    condition_expression<expression_value, bool> *expression;

public:
    condition_register_status_flag(emulation_devices *device, string _name, json condition);
    string get_name();
    status_flag_type get_type();
    bool get_value();
    condition_expression<expression_value, bool> *get_expression();
};
