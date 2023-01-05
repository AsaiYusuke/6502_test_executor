#pragma once

#include <map>
#include <string>

using namespace std;

enum class operator_type
{
    // Any of
    ANY_OF,
    // Equal to
    EQ,
    // Greater than or equal to
    GE,
    // Greater than
    GT,
    // Less than or equal to
    LE,
    // Less than
    LT,
    // Not equal to
    NE
};

static map<string, operator_type> operator_name_type_map = {
    {"anyOf", operator_type::ANY_OF},
    {"eq", operator_type::EQ},
    {"ge", operator_type::GE},
    {"gt", operator_type::GT},
    {"le", operator_type::LE},
    {"lt", operator_type::LT},
    {"ne", operator_type::NE}};

static map<operator_type, string> operator_type_name_map = {
    {operator_type::ANY_OF, "anyOf"},
    {operator_type::EQ, "eq"},
    {operator_type::GE, "ge"},
    {operator_type::GT, "gt"},
    {operator_type::LE, "le"},
    {operator_type::LT, "lt"},
    {operator_type::NE, "ne"}};
