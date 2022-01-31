#include <stdio.h>
#include <iostream>
#include <fstream>

#include "memory_device.h"

void memory_device::load_rom_image(string path)
{
    streampos size;

    ifstream file(path, ios::in | ios::binary | ios::ate);
    if (!file.is_open())
    {
        cerr << "Unable to open Program file: " << path << endl;
        throw 1;
    }
    size = file.tellg();
    rom = new char[size];
    file.seekg(0, ios::beg);
    file.read(rom, size);
    file.close();
}

void memory_device::load_symbol_defs(string path)
{
    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Unable to open Symbol file: " << path << endl;
        throw 1;
    }

    string line;
    while (getline(file, line))
        parse_symbol_def(line);
    file.close();
}

uint16_t memory_device::memory_offset(uint16_t address)
{
    return address - 0x8000 + 0x10;
}

void memory_device::parse_symbol_def(string line)
{
    if (line.substr(0, 4).compare("sym\t") != 0)
        return;

    auto name_pos = line.find("name=\"");
    if (name_pos == string::npos)
        return;

    name_pos += 6;

    auto name_end_pos = line.find("\"", name_pos);
    if (name_end_pos == string::npos)
        return;

    auto val_pos = line.find("val=0x");
    if (val_pos == string::npos)
        return;

    val_pos += 6;

    auto val_end_pos = line.find(",", val_pos);
    if (val_end_pos == string::npos)
        return;

    auto label = line.substr(name_pos, name_end_pos - name_pos);
    auto value = stoi(line.substr(val_pos, val_end_pos - val_pos), 0, 16);
    label_address_map[label] = value;
    address_label_map[value] = label;
}

memory_device::memory_device(string program_path, string symbol_path)
{
    load_rom_image(program_path);
    load_symbol_defs(symbol_path);
}

void memory_device::clear()
{
    ram.clear();
    read_sequences.clear();
    write_sequences.clear();
    read_counts.clear();
    write_counts.clear();
}

bool memory_device::has_address(string label)
{
    return label_address_map.count(label) > 0;
}

uint16_t memory_device::get_address(string label)
{
    return label_address_map[label];
}

string memory_device::get_label(uint16_t address)
{
    return address_label_map[address];
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
        printf("  %s($%X) : $%X\n", address_label_map[k].c_str(), k, v);
    }
}
