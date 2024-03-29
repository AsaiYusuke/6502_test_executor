#pragma once

#include "nlohmann/json.hpp"

#include "condition/condition_expression.hpp"
#include "condition/expression_value.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition_timeout
{
private:
    condition_expression<expression_value, bool> *expression;

public:
    condition_timeout(const emulation_devices *device, json condition);
    condition_expression<expression_value, bool> *get_expression() const;
};
