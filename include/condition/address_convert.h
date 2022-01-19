#pragma once

#include <stdint.h>
#include "emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class address_convert
{
protected:
    uint16_t get_address(emulation_devices *device, json value);
    uint8_t to_byte(emulation_devices *device, json value);
};
