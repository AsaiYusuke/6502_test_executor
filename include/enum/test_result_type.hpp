#pragma once

#include <map>
#include <string>

using namespace std;

enum class test_result_type
{
    // OK
    OK,
    // FAIL
    FAIL,
    // SKIP
    SKIP
};

static map<test_result_type, string> result_type_name_map = {
    {test_result_type::OK, "OK"},
    {test_result_type::FAIL, "FAIL"},
    {test_result_type::SKIP, "SKIP"}};
