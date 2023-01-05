#pragma once

#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class emulation_devices;

class expression_value
{
public:
    template <typename U>
    static U convert(emulation_devices *device, json condition)
    {
        return condition.get<U>();
    }
};
