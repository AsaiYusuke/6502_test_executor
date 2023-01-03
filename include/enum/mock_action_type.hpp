#pragma once

#include <map>
#include <string>

using namespace std;

enum class mock_action_type
{
    // RTS
    RTS,
    // JMP
    JMP
};

static map<string, mock_action_type> mock_action_name_type_map = {
    {"RTS", mock_action_type::RTS},
    {"JMP", mock_action_type::JMP}};
