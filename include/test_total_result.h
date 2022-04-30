#pragma once

#include "args_parser.h"
#include "test_result.h"

using namespace std;

class test_total_result
{
private:
    emulation_devices *device;
    args_parser *args;
    int total;
    vector<test_result> results;
    map<test_result_type, int> test_result_map;

public:
    test_total_result(emulation_devices *_device, args_parser *_args, int _total);
    void add_and_print_result(test_result result);
    bool is_success();
    void print_summary();
};
