#pragma once

#include "enum/status_flag_type.h"
#include "condition/message_name.h"
#include "condition/condition_register_status_flag_value.h"
#include "condition/condition_register_status_flag_count.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition_register_status_flag : public i_message_name
{
private:
    string name;
    status_flag_type type;
    condition_register_status_flag_value *value;
    condition_register_status_flag_count *read_count;
    condition_register_status_flag_count *write_count;

public:
    condition_register_status_flag(emulation_devices *device, string _name, json condition);
    string get_name();
    status_flag_type get_type();
    condition_register_status_flag_value *get_value();
    condition_register_status_flag_count *get_read_count();
    condition_register_status_flag_count *get_write_count();
};
