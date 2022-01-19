#pragma once

#include <map>
#include <string>

using namespace std;

enum class status_flag_type
{
    Negative /*  */ = 0b10000000,
    Overflow /*  */ = 0b01000000,
    Constant /*  */ = 0b00100000,
    Break /*     */ = 0b00010000,
    Decimal /*   */ = 0b00001000,
    Interrupt /* */ = 0b00000100,
    Zero /*      */ = 0b00000010,
    Carry /*     */ = 0b00000001,
    LastEnum /*  */,
};

static map<string, status_flag_type> status_flag_name_type_map = {
    {"Negative", status_flag_type::Negative},
    {"Overflow", status_flag_type::Overflow},
    {"Constant", status_flag_type::Constant},
    {"Break", status_flag_type::Break},
    {"Decimal", status_flag_type::Decimal},
    {"Interrupt", status_flag_type::Interrupt},
    {"Zero", status_flag_type::Zero},
    {"Carry", status_flag_type::Carry}};
