#pragma once

#include <map>
#include <string>

using namespace std;

enum class interrupt_type
{
    // NMI
    NMI,
    // IRQ
    IRQ
};

static map<string, interrupt_type> interrupt_name_type_map = {
    {"NMI", interrupt_type::NMI},
    {"IRQ", interrupt_type::IRQ}};
