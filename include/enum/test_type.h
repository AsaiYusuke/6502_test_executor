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
    // IRQ
    IRQ,
    // ADDRESS
    ADDRESS
};

static map<string, test_type> test_end_name_type_map = {
    {"JSR", test_type::JSR},
    {"NMI", test_type::NMI},
    {"IRQ", test_type::IRQ},
    {"address", test_type::ADDRESS}};
