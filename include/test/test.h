#pragma once

#include "test/test_result.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class args_parser;
class emulation_devices;
class test_total_result;
class debug_info;

class test
{
private:
    args_parser *args;
    debug_info *debug;
    emulation_devices *device;
    vector<json> test_scenarios;

    json read_json(string path);
    int traverse(emulation_devices *device, test_total_result *total_result, json test_target, json test_template, json testcase, string path);
    test_result do_test(string id, json test_target, json test_template, json testcase);
    void print_call_stack();

public:
    test(args_parser *args);
    bool execute();
};
