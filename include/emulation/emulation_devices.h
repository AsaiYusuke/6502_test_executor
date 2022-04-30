#pragma once

#include <stdio.h>
#include <map>

#include "nlohmann/json.hpp"
#include "args_parser.h"
#include "cpu_device.h"
#include "memory_device.h"
#include "runtime_error_result.h"

using namespace std;

using json = nlohmann::json;

class emulation_devices
{
private:
    cpu_device *cpu;
    memory_device *memory;
    vector<runtime_error_result> errors;

public:
    emulation_devices(args_parser *args, json config);
    void clear(uint16_t startPC, uint16_t endPC, vector<uint8_t> stack);
    cpu_device *get_cpu();
    memory_device *get_memory();

    uint16_t get_address(string label);

    bool is_digits(const string &str);
    uint16_t two_complement_byte(uint16_t value);
    void add_error_reuslt(runtime_error_type type);
    void add_error_reuslt(runtime_error_type type, string message);
    vector<runtime_error_result> get_filtered_errors(vector<runtime_error_type> types);

    void print();
};
