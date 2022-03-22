#pragma once

#include "nlohmann/json.hpp"
#include "emulation/emulation_devices.h"

using namespace std;

using json = nlohmann::json;

class expression_value
{
public:
    template <typename U>
    static U convert(emulation_devices *device, json condition)
    {
        return condition.get<U>();
    }
};
