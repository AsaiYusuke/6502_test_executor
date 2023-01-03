#pragma once

#include <string>
#include <vector>

#include "enum/runtime_error_type.hpp"

using namespace std;

class runtime_error_result
{
private:
    runtime_error_type type;
    string message;
    vector<uint16_t> call_stack;

public:
    runtime_error_result(runtime_error_type _type, string _message, vector<uint16_t> _call_stack);
    runtime_error_type get_type();
    string get_message();
    vector<uint16_t> get_call_stack();
};
