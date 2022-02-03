#include <fstream>

#include "memory_device.h"
#include "exception/file_open.h"

void memory_device::load_rom_image(string path)
{
    streampos size;

    ifstream file(path, ios::in | ios::binary | ios::ate);
    if (!file.is_open())
        throw file_open_error(path);

    size = file.tellg();
    rom = new char[size];
    file.seekg(0, ios::beg);
    file.read(rom, size);
    file.close();
}

uint16_t memory_device::memory_offset(uint16_t address)
{
    return address - 0x8000 + 0x10;
}

memory_device::memory_device(string program_path, string debug_path)
{
    load_rom_image(program_path);
    debug = new debug_info(debug_path);
}

void memory_device::clear()
{
    ram.clear();
    read_sequences.clear();
    write_sequences.clear();
    read_counts.clear();
    write_counts.clear();
}

string memory_device::get_source_line(uint16_t address)
{
    return debug->get_source_line(address);
}

bool memory_device::has_label(string label)
{
    return debug->has_label(label);
}

uint16_t memory_device::get_address(string label)
{
    return debug->get_address(label);
}

string memory_device::get_label(uint16_t address)
{
    return debug->get_label(address);
}

void memory_device::set_read_sequence(uint16_t address, vector<uint8_t> sequence)
{
    read_sequences[address] = sequence;
}

vector<uint8_t> memory_device::get_write_sequence(uint16_t address, size_t length)
{
    if (write_sequences.count(address) > 0)
    {
        vector<uint8_t> result(write_sequences[address].end() - length, write_sequences[address].end());
        return result;
    }
    return {read(address)};
}

uint8_t memory_device::get_read_count(uint16_t address)
{
    return read_counts[address];
}

uint8_t memory_device::get_write_count(uint16_t address)
{
    return write_counts[address];
}

uint8_t memory_device::read(uint16_t address)
{
    read_counts[address]++;

    uint8_t val;
    if (address >= 0x8000)
    {
        val = rom[memory_offset(address)];
    }
    else
    {
        if (read_sequences.count(address) > 0)
        {
            ram[address] = read_sequences[address].front();
            read_sequences[address].erase(read_sequences[address].begin());
            if (read_sequences[address].size() == 0)
                read_sequences.erase(address);
        }
        val = ram[address];
    }

    return val;
}

void memory_device::write(uint16_t address, uint8_t value)
{
    write_counts[address]++;

    if (write_sequences.count(address) == 0)
        write_sequences[address] = {};

    write_sequences[address].push_back(value);

    ram[address] = value;
}

void memory_device::print()
{
    printf("MEMORY result:\n");
    for (map<uint16_t, uint8_t>::iterator iter = ram.begin(); iter != ram.end(); ++iter)
    {
        uint16_t k = iter->first;
        uint8_t v = iter->second;
        printf("  %s($%X) : $%X\n", debug->get_label(k).c_str(), k, v);
    }
}
