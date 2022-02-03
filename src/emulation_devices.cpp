#include <stdexcept>

#include "emulation_devices.h"

emulation_devices::emulation_devices(string program_path, string debug_path)
{
    memory = new memory_device(program_path, debug_path);
    cpu = new cpu_device(memory);
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
    if (!memory->has_label(label))
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

uint8_t emulation_devices::two_complement_byte(uint8_t value)
{
    uint8_t result = value;
    if (result < 0)
        result += 256;

    return result;
}

uint8_t emulation_devices::to_byte(string str)
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