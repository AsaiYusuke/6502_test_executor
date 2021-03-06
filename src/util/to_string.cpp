#include <vector>
#include <sstream>

#include "util/to_string.h"
#include "util/value_convert.h"
#include "enum/operator_type.h"

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
