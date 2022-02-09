#include "assert/assert_register_value.h"
#include "assert/operation/assert_equal.h"

bool assert_register_value::test(emulation_devices *device, condition_register register_def, vector<string> &errors)
{
    cpu_device *cpu_dev = device->get_cpu();

    if (register_def.is_blank())
        return true;

    return assert_equal::test(
        register_def.get_value(),
        cpu_dev->get_register(register_def.get_type()),
        "Register [" + string(register_type_name_map[register_def.get_type()]) + "]",
        errors);
}
