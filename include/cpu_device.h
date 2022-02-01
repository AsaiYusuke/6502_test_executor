#pragma once

#include "register_type.h"
#include "mos6502.h"
#include "memory_access.h"

using namespace std;

#define DEFAULT_TIMEOUT 100000

class cpu_device
{
private:
    mos6502 *cpu;

public:
    cpu_device(i_memory_access *memory_access);
    void clear(uint16_t target_program_counter);
    void execute();
    int get_timeout();

    uint8_t get_register(register_type type);
    void set_register(register_type type, uint8_t value);

    void print();
};
