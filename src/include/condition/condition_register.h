#pragma once

#include <stdint.h>
#include "register_type.h"
#include "condition/address_convert.h"
#include "../../../../json/single_include/nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class condition_register : public address_convert
{
private:
    register_type type;
    bool blank;
    uint8_t value;

public:
    condition_register(register_type type, emulation_devices *_device, json condition);
    register_type get_type();
    bool is_blank();
    uint8_t get_value();
};
