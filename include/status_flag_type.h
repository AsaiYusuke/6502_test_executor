#pragma once

#include <map>
#include <string>

#include "emulation/mos6502.h"

using namespace std;

enum class status_flag_type
{
    // Negative Flag
    Negative /*         */ = NEGATIVE,
    // Overflow
    Overflow /*         */ = OVERFLOW,
    // Constant
    Constant /*         */ = CONSTANT,
    // Break Command
    Break /*            */ = BREAK,
    // Decimal Mode Flag
    Decimal /*          */ = DECIMAL,
    // Interrupt Disable
    InterruptDisable /* */ = INTERRUPT,
    // Zero Flag
    Zero /*             */ = ZERO,
    // Carry Flag
    Carry /*            */ = CARRY
};

static map<string, status_flag_type> status_flag_name_type_map = {
    {"Negative", status_flag_type::Negative},
    {"Overflow", status_flag_type::Overflow},
    {"Constant", status_flag_type::Constant},
    {"Break", status_flag_type::Break},
    {"Decimal", status_flag_type::Decimal},
    {"InterruptDisable", status_flag_type::InterruptDisable},
    {"Zero", status_flag_type::Zero},
    {"Carry", status_flag_type::Carry}};
