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
    // Illegal instruction
    ILLEGAL_INSTRUCTION
};
