#pragma once

#include <cstdint>

#include "nlohmann/json.hpp"

#include "enum/interrupt_type.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class condition_interrupt
{
private:
    interrupt_type type;
    uint16_t entryPoint;

public:
    condition_interrupt(const emulation_devices *_device, json condition);
    interrupt_type get_type() const;
    uint16_t get_entry_point() const;
};
