#pragma once

#include "status_flag_type.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_status_register
{
private:
    bool blank;
    vector<tuple<status_flag_type, bool, string>> flags;

public:
    condition_status_register(json condition);
    vector<tuple<status_flag_type, bool, string>> get_flags();
};
