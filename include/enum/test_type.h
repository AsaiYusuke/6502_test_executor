#pragma once

#include <map>
#include <string>

using namespace std;

enum class test_type
{
    // JSR
    JSR,
    // NMI
    NMI,
    // ADDRESS
    ADDRESS
};

static map<string, test_type> test_end_name_type_map = {
    {"jsr", test_type::JSR},
    {"nmi", test_type::NMI},
    {"address", test_type::ADDRESS}};
