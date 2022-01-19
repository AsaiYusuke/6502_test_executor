#pragma once

#include <stdint.h>
#include "emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_pc_register
{
private:
    uint16_t address;

public:
    condition_pc_register(emulation_devices *_device, json condition);
    uint16_t get_address();
};
