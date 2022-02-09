#include "assert/assert_status_flag_value.h"
#include "assert/operation/assert_equal.h"

bool assert_status_flag_value::test(emulation_devices *device, condition_register_status_flag status_flag_def, vector<string> &errors)
{
    cpu_device *cpu_dev = device->get_cpu();
    uint8_t status = cpu_dev->get_register(register_type::Status);
    return assert_equal::test(
        status_flag_def.get_value(),
        (status & (uint8_t)status_flag_def.get_type()) > 0,
        "Register [Status (" + status_flag_def.get_name() + ")]",
        errors);
}
