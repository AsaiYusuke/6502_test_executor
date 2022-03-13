#pragma once

#include "enum/operator_type.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_timeout
{
private:
    vector<pair<operator_type, bool>> expressions;

public:
    condition_timeout(json condition);
    vector<pair<operator_type, bool>> get_expressions();
};
