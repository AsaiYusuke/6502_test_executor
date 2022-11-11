#pragma once

#include <stdint.h>
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class value_convert
{
public:
    static uint16_t get_address(emulation_devices *device, json value);
    static uint16_t parse_json_number(emulation_devices *device, json value);
    static uint16_t to_two_complement_byte(emulation_devices *device, json value);
    static string to_hex_string(uint16_t value);
    static string to_zero_filled_hex_string(uint16_t value);
    static json parse_variable(json definitions_def, json value);
    static json parse_template(json definitions_def, json value);
    static json parse_all_variable(json definitions_def, json condition);
    static json merge_template(json templates, json value);
};
