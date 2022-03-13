#pragma once

#include "enum/operator_type.h"
#include "enum/status_flag_type.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_register_status_flag
{
private:
    string name;
    status_flag_type type;
    bool value;
    vector<pair<operator_type, bool>> expressions;

public:
    condition_register_status_flag(emulation_devices *_device, string _name, json condition);
    string get_name();
    status_flag_type get_type();
    bool get_value();
    vector<pair<operator_type, bool>> get_expressions();
};
