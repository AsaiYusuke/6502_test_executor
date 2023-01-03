#pragma once

#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class test_detect
{
private:
    static bool key_type_check(string key, json::value_t type, json value);

public:
    static bool is_project(json value);
    static bool is_file_reference(json value);
};
