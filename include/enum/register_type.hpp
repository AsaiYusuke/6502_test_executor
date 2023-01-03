#pragma once

#include <map>
#include <string>

using namespace std;

enum class register_type
{
    // Accumulator
    A,
    // Index register X
    X,
    // Index register Y
    Y,
    // Processor status register
    P,
    // Stack pointer
    S,
    // Program Counter
    PC
};

static map<string, register_type> register_name_type_map = {
    {"A", register_type::A},
    {"X", register_type::X},
    {"Y", register_type::Y},
    {"P", register_type::P},
    {"S", register_type::S},
    {"PC", register_type::PC}};
