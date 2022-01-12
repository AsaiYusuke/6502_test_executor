#ifndef __EMULATION_DEVICES_H_INCLUDED__
#define __EMULATION_DEVICES_H_INCLUDED__

#include <stdio.h>
#include <map>

#include "cpu_device.h"
#include "memory_device.h"

using namespace std;

class emulation_devices
{
private:
    cpu_device *cpu;
    memory_device *memory;

public:
    emulation_devices(string program_path, string symbol_path);
    void clear(uint16_t target_program_counter);
    cpu_device *get_cpu();
    memory_device *get_memory();

    uint16_t get_address(string label);

    bool is_digits(const string &str);
    uint8_t two_complement_byte(uint8_t value);
    uint8_t to_byte(string str);

    void print();
};

#endif
