#include "condition/condition_register_a_x_y.h"
#include "util/value_convert.h"
#include "util/expression_executer.h"
#include <iostream>

condition_register_a_x_y::condition_register_a_x_y(emulation_devices *device, string _name, json condition)
{
    name = _name;
    type = register_name_type_map[name];
    value = new condition_register_a_x_y_value(device, condition["value"]);
    read_count = new condition_register_a_x_y_count(device, condition["readCount"]);
    write_count = new condition_register_a_x_y_count(device, condition["writeCount"]);
}

string condition_register_a_x_y::get_name()
{
    return name;
}

register_type condition_register_a_x_y::get_type()
{
    return type;
}

condition_register_a_x_y_value *condition_register_a_x_y::get_value()
{
    return value;
}

condition_register_a_x_y_count *condition_register_a_x_y::get_read_count()
{
    return read_count;
}

condition_register_a_x_y_count *condition_register_a_x_y::get_write_count()
{
    return write_count;
}
