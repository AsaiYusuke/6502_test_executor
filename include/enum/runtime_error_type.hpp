#pragma once

using namespace std;

enum class runtime_error_type
{
    // Timeout
    TIMEOUT,
    // Attempted write to readonly memory
    READONLY_MEMORY,
    // Out of segment
    OUT_OF_SEGMENT,
    // Attempted read from uninitialized memory
    UNINITIALIZED_MEMORY,
    // Attempted access to unauthorized memory
    UNAUTHORIZED_MEMORY,
    // Illegal instruction
    ILLEGAL_INSTRUCTION
};
