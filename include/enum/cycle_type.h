#pragma once

#include <bits/stdc++.h>
#include <map>
#include <string>

using namespace std;

enum class cycle_type
{
    // NES NTSC Vsync
    NES_NTSC_VSYNC,
    // Disable max cycle
    DISABLE
};

static map<string, cycle_type> cycle_name_type_map = {
    {"NES_NTSC_VSYNC", cycle_type::NES_NTSC_VSYNC},
    {"DISABLE", cycle_type::DISABLE}};

static map<cycle_type, uint64_t> cycle_type_value_map = {
    {cycle_type::NES_NTSC_VSYNC, 29784},
    {cycle_type::DISABLE, INT_MAX}};
