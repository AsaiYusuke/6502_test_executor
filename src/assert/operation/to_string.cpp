#include <vector>
#include <sstream>

#include "assert/operation/to_string.h"
#include "condition/address_convert.h"

string to_string(vector<uint8_t> values)
{
    stringstream ss;
    ss << "[";

    ss << address_convert::to_hex_string(values[0]);
    for (decltype(values.size()) index = 1, size = values.size(); index < size; index++)
    {
        ss << ", " << address_convert::to_hex_string(values[index]);
    }

    ss << "]";
    return ss.str();
}

string to_string(bool value)
{
    return value ? "True" : "False";
}
