#pragma once

#include "nlohmann/json.hpp"
#include "emulation/emulation_devices.h"
#include "util/value_convert.h"

using namespace std;

using json = nlohmann::json;

class expression_two_complement_byte
{
public:
    template <typename U>
    static U convert(emulation_devices *device, json condition)
    {
        return value_convert::to_two_complement_byte(device, condition);
    }
};
