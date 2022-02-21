#pragma once

#include "args_parser.h"
#include "test_result.h"
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

    test_result do_test(json testcase);
    void print_test_result(string test_name, test_result result);
    void print_summary(map<test_result_type, int> result_map, int total);
    void print_call_stack();

public:
    test(args_parser *args);
    bool execute();
}; 
