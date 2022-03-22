#pragma once

#include "enum/interrupt_type.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_interrupt
{
private:
    interrupt_type type;
    uint8_t hook_address;

public:
    condition_interrupt(emulation_devices *_device, json condition);
    interrupt_type get_type();
    uint8_t get_hook_address();
};
