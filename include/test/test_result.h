#pragma once

#include <vector>
#include <string>

#include "enum/test_result_type.h"

using namespace std;

class emulation_devices;

class test_result
{
private:
    string id;
    test_result_type result_type;
    vector<string> errors;

    test_result(test_result_type type);

public:
    test_result();
    void set_id(string id);
    void add_error(string error);
    test_result_type get_result_type();
    string get_result_name();
    vector<string> get_errors();
    static test_result skip();
    void print_result(emulation_devices *device);
};
