#include "assert/assert_timeout.h"
#include "assert/operation/assert_equal.h"

bool assert_timeout::test(emulation_devices *device, bool expected, vector<string> &errors)
{
    cpu_device *cpu_dev = device->get_cpu();
    return assert_equal::test(
        expected,
        cpu_dev->is_timeout(),
        "Timeout [execution count >= " + to_string(cpu_dev->get_timeout_threshold()) + "]",
        errors);
}
