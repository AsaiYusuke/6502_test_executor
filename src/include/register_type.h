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

static map<register_type, string> register_type_name_map = {
    {register_type::A, "A"},
    {register_type::X, "X"},
    {register_type::Y, "Y"},
    {register_type::Status, "Status"},
    {register_type::PC, "PC"}};
