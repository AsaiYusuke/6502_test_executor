#pragma once

#include "condition/condition_expression.h"
#include "condition/expression_value.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition_timeout
{
private:
    condition_expression<expression_value, bool> *expression;

public:
    condition_timeout(emulation_devices *device, json condition);
    condition_expression<expression_value, bool> *get_expression();
};
