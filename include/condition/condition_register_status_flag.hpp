#pragma once

#include "nlohmann/json.hpp"

#include "condition/message_name.hpp"
#include "enum/status_flag_type.hpp"

using namespace std;

using json = nlohmann::json;

class condition_register_status_flag_value;
class condition_register_status_flag_count;
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
    condition_register_status_flag(const emulation_devices *device, const string &_name, json condition);
    string get_name() const;
    status_flag_type get_type() const;
    condition_register_status_flag_value *get_value() const;
    condition_register_status_flag_count *get_read_count() const;
    condition_register_status_flag_count *get_write_count() const;
};
