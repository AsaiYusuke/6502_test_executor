#pragma once

#include <stdexcept>
#include <string>

#include "enum/runtime_error_type.h"

using namespace std;

class cpu_runtime_error : public runtime_error
{
private:
    runtime_error_type type;

public:
    cpu_runtime_error(runtime_error_type _type, string message)
        : runtime_error(message)
    {
        type = _type;
    }

    const runtime_error_type get_type() const noexcept
    {
        return type;
    }
};
