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

uint16_t emulation_devices::get_address(string label, int offset)
{
    if (!memory->has_address(label))
        throw invalid_argument("Symbol not found: " + label);

    return memory->get_address(label) + offset;
}

void emulation_devices::print()
{
    cpu->print();
    memory->print();
}

bool emulation_devices::is_digits(const string &str)
{
    return str.find_first_not_of("-0123456789") == string::npos;
}

uint16_t emulation_devices::two_complement_byte(uint16_t value)
{
    uint16_t result = value;
    if (result < 0)
        result += 256;

    return result;
}

uint16_t emulation_devices::to_byte(string str)
{
    int value;

    if (str.compare(0, 1, "$") == 0)
        value = stoi(str.substr(1), 0, 16);
    else if (str.compare(0, 1, "%") == 0)
        value = stoi(str.substr(1), 0, 2);
    else if (is_digits(str))
        value = stoi(str);
    else
        value = get_address(str, 0);

    return two_complement_byte(value);
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
