#include "assert/assert_memory_read_count.h"
#include "assert/operation/assert_equal.h"

bool assert_memory_read_count::test(emulation_devices *device, condition_memory_count memory_count_def, vector<string> &errors)
{
    memory_device *mem_dev = device->get_memory();
    uint8_t expected_value = memory_count_def.get_count();
    return assert_equal::test(
        expected_value,
        mem_dev->get_read_count(memory_count_def.get_address()),
        "Memory data [" + memory_count_def.get_name() + "]",
        errors);
}
