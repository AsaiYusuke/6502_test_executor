#pragma once

#include "test/test_result.h"

using namespace std;

class args_parser;

class test_total_result
{
private:
    args_parser *args;
    int total;
    vector<test_result> results;
    map<test_result_type, int> test_result_map;

public:
    test_total_result(args_parser *_args);
    void add_and_print_result(emulation_devices *device, test_result result);
    bool is_success();
    void set_total(int total);
    void print_summary();
};
