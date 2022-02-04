#pragma once

#include <stdio.h>
#include <map>

#include "nlohmann/json.hpp"
#include "args_parser.h"
#include "cpu_device.h"
#include "memory_device.h"

using namespace std;

using json = nlohmann::json;

class emulation_devices
{
private:
    cpu_device *cpu;
    memory_device *memory;

public:
    emulation_devices(args_parser *args, json config);
    void clear(uint16_t target_program_counter);
    cpu_device *get_cpu();
    memory_device *get_memory();

    uint16_t get_address(string label, int offset);

    bool is_digits(const string &str);
    uint8_t two_complement_byte(uint8_t value);
    uint8_t to_byte(string str);

    void print();
};
