#include "emulation/emulation_devices.hpp"

#include <stdexcept>

#include "assert/runtime_error_result.hpp"
#include "emulation/cpu_device.hpp"
#include "emulation/memory_device.hpp"

emulation_devices::emulation_devices(const args_parser *args, json config, debug_info *debug)
{
    memory = new memory_device(this, args, config, debug);
    cpu = new cpu_device(this, args, config, debug);
}

void emulation_devices::clear(const condition_register_pc *pc, const vector<uint8_t> &stack)
{
    memory->clear();
    cpu->clear(pc, stack);
    errors.clear();
}

cpu_device *emulation_devices::get_cpu() const
{
    return cpu;
}

memory_device *emulation_devices::get_memory() const
{
    return memory;
}

uint16_t emulation_devices::get_address(const string &label) const
{
    return memory->get_address(label);
}

void emulation_devices::print() const
{
    cpu->print();
    memory->print();
}

void emulation_devices::add_error_result(runtime_error_type type, const string &message)
{
    errors.push_back(
        new runtime_error_result(type, message, cpu->get_call_stack()));
}

vector<runtime_error_result *> emulation_devices::get_filtered_errors(const vector<runtime_error_type> &types) const
{
    vector<runtime_error_result *> filtered_result;
    for (auto error : errors)
        if (find(types.begin(), types.end(), error->get_type()) != types.end())
            filtered_result.push_back(error);

    return filtered_result;
}
