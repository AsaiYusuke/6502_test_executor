#pragma once

#include "condition/condition.h"
#include "enum/test_result_type.h"

using namespace std;

class test_result
{
private:
    test_result_type result_type;
    vector<string> errors;

    test_result(test_result_type type);

public:
    test_result();
    void add_error(string error);
    test_result_type get_result_type();
    string get_result_name();
    vector<string> get_errors();
    void print_error();
    static test_result skip();
};
