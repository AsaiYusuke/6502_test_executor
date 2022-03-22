#pragma once

#include "condition/condition_expression.h"
#include "condition/expression_value.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_timeout
{
private:
    condition_expression<expression_value, bool> *expression;

public:
    condition_timeout(emulation_devices *device, json condition);
    condition_expression<expression_value, bool> *get_expression();
};
