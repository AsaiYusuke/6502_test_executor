#pragma once

#include "args_parser.h"
#include "test/test_total_result.h"
#include "test/test_result.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class test
{
private:
    args_parser *args;
    debug_info *debug;
    emulation_devices *device;
    vector<json> test_scinarios;

    json read_json(string path);
    void traverse(test_total_result &total_result, json test_target, json test_template, json testcase, string path);
    test_result do_test(string id, json test_target, json test_template, json testcase);
    void print_call_stack();

public:
    test(args_parser *args);
    bool execute();
};
