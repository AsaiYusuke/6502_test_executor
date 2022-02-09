#pragma once

#include <map>
#include <string>

using namespace std;

enum class register_type
{
    A,
    X,
    Y,
    Status,
    PC
};

static map<string, register_type> register_name_type_map = {
    {"A", register_type::A},
    {"X", register_type::X},
    {"Y", register_type::Y},
    {"Status", register_type::Status},
    {"PC", register_type::PC}};
