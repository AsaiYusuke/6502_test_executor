#include "condition/condition_register_status_flag.hpp"

#include "condition/condition_register_status_flag_count.hpp"
#include "condition/condition_register_status_flag_value.hpp"
#include "util/expression_execute.hpp"
#include "util/value_convert.hpp"

condition_register_status_flag::condition_register_status_flag(const emulation_devices *device, const string &_name, json condition)
{
    name = _name;
    type = status_flag_name_type_map[name];
    value = new condition_register_status_flag_value(device, condition["value"]);
    read_count = new condition_register_status_flag_count(device, condition["readCount"]);
    write_count = new condition_register_status_flag_count(device, condition["writeCount"]);
}

string condition_register_status_flag::get_name() const
{
    return name;
}

status_flag_type condition_register_status_flag::get_type() const
{
    return type;
}

condition_register_status_flag_value *condition_register_status_flag::get_value() const
{
    return value;
}

condition_register_status_flag_count *condition_register_status_flag::get_read_count() const
{
    return read_count;
}

condition_register_status_flag_count *condition_register_status_flag::get_write_count() const
{
    return write_count;
}
