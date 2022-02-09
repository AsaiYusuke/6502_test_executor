#pragma once

#include <stdint.h>
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_register_pc
{
private:
    uint16_t address;

public:
    condition_register_pc(emulation_devices *_device, json condition);
    uint16_t get_address();
};
