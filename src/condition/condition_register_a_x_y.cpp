#include "condition/condition_register_a_x_y.hpp"

#include <iostream>

#include "condition/condition_register_a_x_y_count.hpp"
#include "condition/condition_register_a_x_y_value.hpp"
#include "util/expression_execute.hpp"
#include "util/value_convert.hpp"

condition_register_a_x_y::condition_register_a_x_y(const emulation_devices *device, const string &_name, json condition)
{
    name = _name;
    type = register_name_type_map[name];
    value = new condition_register_a_x_y_value(device, condition["value"]);
    read_count = new condition_register_a_x_y_count(device, condition["readCount"]);
    write_count = new condition_register_a_x_y_count(device, condition["writeCount"]);
}

string condition_register_a_x_y::get_name() const
{
    return name;
}

register_type condition_register_a_x_y::get_type() const
{
    return type;
}

condition_register_a_x_y_value *condition_register_a_x_y::get_value() const
{
    return value;
}

condition_register_a_x_y_count *condition_register_a_x_y::get_read_count() const
{
    return read_count;
}

condition_register_a_x_y_count *condition_register_a_x_y::get_write_count() const
{
    return write_count;
}
