#pragma once

#include <vector>

#include "mos6502.h"
#include "nlohmann/json.hpp"
#include "args_parser.h"
#include "enum/register_type.h"
#include "enum/runtime_error_type.h"
#include "runtime_error_result.h"

#define TEST_RETURN_ADDRESS  0xFFFF

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class cpu_device
{
private:
    emulation_devices *device;

    mos6502 *cpu;
    int timeout_threshold;
    vector<uint16_t> call_stack;
    uint16_t currentPC;

public:
    cpu_device(emulation_devices *_device, args_parser *args, json config);
    void clear(uint16_t target_program_counter);
    void execute();

    int get_timeout_threshold();

    uint8_t get_register(register_type type);
    void set_register(register_type type, uint8_t value);

    void print();
    vector<uint16_t> get_call_stack();
};
