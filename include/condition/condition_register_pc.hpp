#pragma once

#include <cstdint>

#include "nlohmann/json.hpp"

#include "enum/test_type.hpp"

using namespace std;

class emulation_devices;

using json = nlohmann::json;

class condition_register_pc
{
private:
    uint16_t start_address;
    uint16_t end_address;
    test_type type;

public:
    condition_register_pc(const emulation_devices *_device, json condition);
    uint16_t get_start_address() const;
    test_type get_test_type() const;
    uint16_t get_end_address() const;
};
