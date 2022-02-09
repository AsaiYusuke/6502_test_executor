#include "assert/assert_memory_value.h"
#include "assert/operation/assert_equal.h"

bool assert_memory_value::test(emulation_devices *device, condition_memory_value memory_value_def, vector<string> &errors)
{
    memory_device *mem_dev = device->get_memory();
    vector<uint8_t> expected_value = memory_value_def.get_sequence();
    return assert_equal::test(
        expected_value,
        mem_dev->get_write_sequence(
            memory_value_def.get_address(),
            expected_value.size()),
        "Memory data [" + memory_value_def.get_name() + "]",
        errors);
}
