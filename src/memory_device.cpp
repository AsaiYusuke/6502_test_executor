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
    streampos size;

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
    symbol_address_map[label] = value;
    address_symbol_map[value] = label;
}

memory_device::memory_device(string program_path, string symbol_path)
{
    load_rom_image(program_path);
    load_symbol_defs(symbol_path);
}

void memory_device::clear()
{
    ram.clear();
}

bool memory_device::has_address(string label)
{
    return symbol_address_map.count(label) > 0;
}

uint16_t memory_device::get_address(string label)
{
    return symbol_address_map[label];
}

string memory_device::get_symbol(uint16_t address)
{
    return address_symbol_map[address];
}

uint8_t memory_device::read(uint16_t address)
{
    uint8_t val;
    if (address >= 0x8000)
    {
        val = rom[memory_offset(address)];
    }
    else
    {
        val = ram[address];
    }

    return val;
}

void memory_device::write(uint16_t address, uint8_t value)
{
    ram[address] = value;
}

void memory_device::print()
{
    printf("MEMORY result:\n");
    for (map<uint16_t, uint8_t>::iterator iter = ram.begin(); iter != ram.end(); ++iter)
    {
        uint16_t k = iter->first;
        uint8_t v = iter->second;
        printf("- %s($%x) : $%x\n", address_symbol_map[k].c_str(), k, v);
    }
}
