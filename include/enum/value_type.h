#pragma once

#include <map>
#include <string>

using namespace std;

enum class value_type
{
    // Value
    VALUE = 1,
    // High byte
    HIBYTE,
    // Low byte
    LOBYTE,
    // High byte of rts pointer
    RTS_HIBYTE,
    // Low byte of rts pointer
    RTS_LOBYTE,
};

static map<string, value_type> value_name_type_map = {
    {"value", value_type::VALUE},
    {"hibyte", value_type::HIBYTE},
    {"lobyte", value_type::LOBYTE},
    {"rts_hibyte", value_type::RTS_HIBYTE},
    {"rts_lobyte", value_type::RTS_LOBYTE}};
