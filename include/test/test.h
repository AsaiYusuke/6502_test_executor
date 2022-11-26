#pragma once

#include "args_parser.h"
#include "test/test_result.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class test
{
private:
    args_parser *args;
    emulation_devices *device;
    json test_scinario;

    test_result do_test(string id, json testcase);
    void print_call_stack();

public:
    test(args_parser *args);
    bool execute();
};
