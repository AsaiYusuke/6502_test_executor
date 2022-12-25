#include "condition/condition_register_status_flag.h"
#include "util/expression_execute.h"
#include "util/value_convert.h"

condition_register_status_flag::condition_register_status_flag(emulation_devices *device, string _name, json condition)
{
    name = _name;
    type = status_flag_name_type_map[name];
    value = new condition_register_status_flag_value(device, condition["value"]);
    read_count = new condition_register_status_flag_count(device, condition["readCount"]);
    write_count = new condition_register_status_flag_count(device, condition["writeCount"]);
}

string condition_register_status_flag::get_name()
{
    return name;
}

status_flag_type condition_register_status_flag::get_type()
{
    return type;
}

condition_register_status_flag_value *condition_register_status_flag::get_value()
{
    return value;
}

condition_register_status_flag_count *condition_register_status_flag::get_read_count()
{
    return read_count;
}

condition_register_status_flag_count *condition_register_status_flag::get_write_count()
{
    return write_count;
}
