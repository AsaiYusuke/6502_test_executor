#include "condition/condition_status_flag.h"

condition_status_flag::condition_status_flag(emulation_devices *_device, string _name, json condition)
{
    name = _name;
    type = status_flag_name_type_map[name];
    blank = condition.is_null();
    if (!blank)
        value = condition.get<bool>();
}

string condition_status_flag::get_name()
{
    return name;
}

status_flag_type condition_status_flag::get_type()
{
    return type;
}

bool condition_status_flag::is_blank()
{
    return blank;
}

bool condition_status_flag::get_value()
{
    return value;
}
