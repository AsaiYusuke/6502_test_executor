#pragma once

#include <stdint.h>
#include "emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class address_convert
{
public:
    static uint16_t get_address(emulation_devices *device, json value);
    static uint8_t to_byte(emulation_devices *device, json value);
    static uint8_t to_two_complement_byte(emulation_devices *device, json value);
    static string to_hex_string(uint16_t value);
};
