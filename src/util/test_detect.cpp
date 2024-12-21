#include "util/test_detect.hpp"

using namespace std;

bool test_detect::key_type_check(string key, json::value_t type, json value)
{
    if (!value.is_object())
        throw invalid_argument("Invalid format: " + to_string(value));

    if (value[key].type() == type)
        return true;

    if (value[key].is_null())
        return false;

    throw invalid_argument("Invalid format: " + to_string(value));
}

bool test_detect::is_project(json value)
{
    return key_type_check("tests", json::value_t::array, value);
}

bool test_detect::is_file_reference(json value)
{
    return key_type_check("file", json::value_t::string, value);
}
