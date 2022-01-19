#pragma once

#include <stdio.h>
#include <map>
#include <stdint.h>
#include <string>

#include "memory_access.h"

using namespace std;

class memory_device : public i_memory_access
{
private:
    char *rom;
    map<uint16_t, uint8_t> ram;

    map<string, uint16_t> label_address_map;
    map<uint16_t, string> address_label_map;

    void load_rom_image(string path);
    void load_symbol_defs(string path);

    uint16_t memory_offset(uint16_t address);
    void parse_symbol_def(string line);

public:
    memory_device(string program_path, string symbol_path);
    void clear();
    bool has_address(string label);
    uint16_t get_address(string label);
    string get_label(uint16_t address);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void print();
};
