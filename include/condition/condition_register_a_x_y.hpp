#pragma once

#include "nlohmann/json.hpp"

#include "condition/message_name.hpp"
#include "enum/register_type.hpp"

using namespace std;

using json = nlohmann::json;

class condition_register_a_x_y_value;
class condition_register_a_x_y_count;
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
    condition_register_a_x_y(const emulation_devices *_device, const string &_name, json condition);
    string get_name() const;
    register_type get_type() const;
    condition_register_a_x_y_value *get_value() const;
    condition_register_a_x_y_count *get_read_count() const;
    condition_register_a_x_y_count *get_write_count() const;
};
