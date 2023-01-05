#include "util/to_string.hpp"

#include <vector>
#include <sstream>

#include "enum/operator_type.hpp"
#include "util/value_convert.hpp"

string to_string(uint8_t value)
{
    return value_convert::to_hex_string(value);
}

string to_string(bool value)
{
    return value ? "True" : "False";
}

string to_string(operator_type value)
{
    return operator_type_name_map[value];
}
