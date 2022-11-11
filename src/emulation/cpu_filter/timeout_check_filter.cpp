#include "emulation/cpu_filter/timeout_check_filter.h"

timeout_check_filter::timeout_check_filter(cpu_device *cpu)
{
    this->cpu = cpu;
}

void timeout_check_filter::clear()
{
}

bool timeout_check_filter::pre()
{
    return true;
}

bool timeout_check_filter::post()
{
    if (cpu->get_cycle_count() > cpu->get_max_cycle_count())
    {
        cpu->add_error_result(runtime_error_type::TIMEOUT);
        return false;
    }

    return true;
}