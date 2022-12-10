#include <stdexcept>

#include "emulation/emulation_devices.h"

emulation_devices::emulation_devices(args_parser *args, json config)
{
    debug = new debug_info(args, config);
    memory = new memory_device(this, args, config, debug);
    cpu = new cpu_device(this, args, config, debug);
}

void emulation_devices::clear(condition_register_pc *pc, vector<uint8_t> stack)
{
    memory->clear();
    cpu->clear(pc, stack);
    errors.clear();
}

cpu_device *emulation_devices::get_cpu()
{
    return cpu;
}

memory_device *emulation_devices::get_memory()
{
    return memory;
}

uint16_t emulation_devices::get_address(string label)
{
    return memory->get_address(label);
}

void emulation_devices::save_coverage()
{
    debug->save_coverage();
}

void emulation_devices::print()
{
    cpu->print();
    memory->print();
}

void emulation_devices::add_error_result(runtime_error_type type, string message)
{
    errors.push_back(
        runtime_error_result(type, message, cpu->get_call_stack()));
}

vector<runtime_error_result> emulation_devices::get_filtered_errors(vector<runtime_error_type> types)
{
    vector<runtime_error_result> filtered_result;
    for (auto error : errors)
        if (find(types.begin(), types.end(), error.get_type()) != types.end())
            filtered_result.push_back(error);

    return filtered_result;
}
