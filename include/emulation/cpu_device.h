#pragma once

#include <vector>

#include "mos6502.h"
#include "nlohmann/json.hpp"
#include "args_parser.h"
#include "memory_access.h"
#include "register_type.h"

using namespace std;

using json = nlohmann::json;

class cpu_device
{
private:
    mos6502 *cpu;
    int timeout_threshold;
    vector<uint16_t> callStack;
    bool timeout;

public:
    cpu_device(args_parser *args, json config, i_memory_access *memory_access);
    void clear(uint16_t target_program_counter);
    void execute();

    bool is_timeout();
    int get_timeout_threshold();

    uint8_t get_register(register_type type);
    void set_register(register_type type, uint8_t value);

    void print();
    vector<uint16_t> get_call_stack();
};
