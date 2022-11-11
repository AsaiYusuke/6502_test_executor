#pragma once

#include "enum/register_type.h"
#include "condition/message_name.h"
#include "condition/condition_register_a_x_y_value.h"
#include "condition/condition_register_a_x_y_count.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition_register_a_x_y : public i_message_name
{
private:
    string name;
    register_type type;
    condition_register_a_x_y_value *value;
    condition_register_a_x_y_count *read_count;
    condition_register_a_x_y_count *write_count;

public:
    condition_register_a_x_y(emulation_devices *_device, string _name, json condition);
    string get_name();
    register_type get_type();
    condition_register_a_x_y_value *get_value();
    condition_register_a_x_y_count *get_read_count();
    condition_register_a_x_y_count *get_write_count();
};
