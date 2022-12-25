#include "emulation/cpu_filter/coverage_filter.h"
#include "emulation/cpu_device.h"
#include "emulation/debug_info.h"

coverage_filter::coverage_filter(cpu_device *cpu)
{
    this->cpu = cpu;
}

void coverage_filter::clear()
{
}

bool coverage_filter::pre()
{
    debug->add_line_coverage(cpu->get_register16(register_type::PC));
    return true;
}

bool coverage_filter::post()
{
    return true;
}

void coverage_filter::set_debug(debug_info *_debug)
{
    debug = _debug;
}