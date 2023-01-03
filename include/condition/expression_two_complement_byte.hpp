#pragma once

#include "nlohmann/json.hpp"

#include "util/value_convert.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class expression_two_complement_byte
{
public:
    template <typename U>
    static U convert(emulation_devices *device, json condition)
    {
        return value_convert::to_two_complement_byte(device, condition);
    }
};
