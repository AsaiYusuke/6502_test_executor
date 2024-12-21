#pragma once

#include <map>

#include "nlohmann/json.hpp"

#include "enum/runtime_error_type.hpp"

using namespace std;

using json = nlohmann::json;

class args_parser;
class cpu_device;
class memory_device;
class debug_info;
class condition_register_pc;
class runtime_error_result;

class emulation_devices
{
private:
    cpu_device *cpu;
    memory_device *memory;
    vector<runtime_error_result *> errors;

public:
    emulation_devices(const args_parser *args, json config, debug_info *debug);
    void clear(const condition_register_pc *pc, const vector<uint8_t> &stack);
    cpu_device *get_cpu() const;
    memory_device *get_memory() const;

    uint16_t get_address(const string &label) const;

    void add_error_result(runtime_error_type type, const string &message);
    vector<runtime_error_result *> get_filtered_errors(const vector<runtime_error_type> &types) const;

    void print() const;
};
