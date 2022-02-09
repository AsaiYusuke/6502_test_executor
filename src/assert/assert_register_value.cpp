#include "assert/assert_register_value.h"
#include "assert/operation/assert_equal.h"

bool assert_register_value::test(emulation_devices *device, condition_register_a_x_y register_def, vector<string> &errors)
{
    cpu_device *cpu_dev = device->get_cpu();

    return assert_equal::test(
        register_def.get_value(),
        cpu_dev->get_register(register_def.get_type()),
        "Register [" + register_def.get_name() + "]",
        errors);
}
