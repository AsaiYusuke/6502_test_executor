#pragma once

#include <map>
#include <string>

using namespace std;

enum class memory_test_type
{
    // WRITE_TO_READONLY_MEMORY
    WRITE_TO_READONLY_MEMORY,
    // ACCESS_TO_OUT_OF_SEGMENT
    ACCESS_TO_OUT_OF_SEGMENT,
    // READ_FROM_UNINITIALIZED_MEMORY
    READ_FROM_UNINITIALIZED_MEMORY,
    // ACCESS_TO_UNAUTHORIZED_MEMORY
    ACCESS_TO_UNAUTHORIZED_MEMORY
};

static map<string, memory_test_type> memory_test_name_type_map = {
    {"WRITE_TO_READONLY_MEMORY", memory_test_type::WRITE_TO_READONLY_MEMORY},
    {"ACCESS_TO_OUT_OF_SEGMENT", memory_test_type::ACCESS_TO_OUT_OF_SEGMENT},
    {"READ_FROM_UNINITIALIZED_MEMORY", memory_test_type::READ_FROM_UNINITIALIZED_MEMORY},
    {"ACCESS_TO_UNAUTHORIZED_MEMORY", memory_test_type::ACCESS_TO_UNAUTHORIZED_MEMORY}};
