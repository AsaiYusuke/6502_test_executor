#include "emulation/cpu_filter/instruction_check_filter.hpp"

#include "emulation/cpu_device.hpp"
#include "util/value_convert.hpp"

instruction_check_filter::instruction_check_filter(cpu_device *_cpu)
{
    cpu = _cpu;
}

void instruction_check_filter::clear()
{
}

bool instruction_check_filter::pre()
{
    pre_pc = cpu->get_register16(register_type::PC);
    return true;
}

bool instruction_check_filter::post()
{
    if (cpu->is_illegal_instruction())
    {
        cpu->add_error_result(
            runtime_error_type::ILLEGAL_INSTRUCTION,
            "address=" + value_convert::to_zero_filled_hex_string(pre_pc));
        return false;
    }

    return true;
}