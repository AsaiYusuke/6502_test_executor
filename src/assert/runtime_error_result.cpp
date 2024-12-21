#include "assert/runtime_error_result.hpp"

#include <string>

using namespace std;

runtime_error_result::runtime_error_result(runtime_error_type _type, const string &_message, const vector<uint16_t> &_call_stack)
{
    type = _type;
    message = _message;
    call_stack = _call_stack;
}

runtime_error_type runtime_error_result::get_type() const
{
    return type;
}

string runtime_error_result::get_message() const
{
    return message;
}

vector<uint16_t> runtime_error_result::get_call_stack() const
{
    return call_stack;
}
