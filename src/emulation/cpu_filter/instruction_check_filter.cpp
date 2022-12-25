#include "emulation/cpu_filter/instruction_check_filter.h"
#include "emulation/cpu_device.h"
#include "util/value_convert.h"

instruction_check_filter::instruction_check_filter(cpu_device *cpu)
{
    this->cpu = cpu;
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