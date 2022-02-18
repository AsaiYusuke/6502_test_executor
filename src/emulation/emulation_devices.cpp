#include <stdexcept>

#include "emulation/emulation_devices.h"

emulation_devices::emulation_devices(args_parser *args, json config)
{
    memory = new memory_device(this, args, config);
    cpu = new cpu_device(this, args, config);
}

void emulation_devices::clear(uint16_t target_program_counter)
{
    memory->clear();
    cpu->clear(target_program_counter);
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

void emulation_devices::print()
{
    cpu->print();
    memory->print();
}

void emulation_devices::add_error_reuslt(runtime_error_type type)
{
    add_error_reuslt(type, "");
}

void emulation_devices::add_error_reuslt(runtime_error_type type, string message)
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
