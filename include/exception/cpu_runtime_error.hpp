#pragma once

#include <stdexcept>
#include <string>

#include "enum/runtime_error_type.hpp"

using namespace std;

class cpu_runtime_error : public runtime_error
{
private:
    runtime_error_type type;

public:
    cpu_runtime_error(runtime_error_type _type, const string &message)
        : runtime_error(message)
    {
        type = _type;
    }

    runtime_error_type get_type() const
    {
        return type;
    }
};
