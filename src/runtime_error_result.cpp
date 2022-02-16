#include <string>
#include "runtime_error_result.h"

runtime_error_result::runtime_error_result(runtime_error_type _type, string _message, vector<uint16_t> _call_stack)
{
    type = _type;
    message = _message;
    call_stack = _call_stack;
}

runtime_error_type runtime_error_result::get_type()
{
    return type;
}

string runtime_error_result::get_message()
{
    return message;
}

vector<uint16_t> runtime_error_result::get_call_stack()
{
    return call_stack;
}
