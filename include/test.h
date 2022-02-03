#pragma once

#include "test_result.h"
#include "emulation/emulation_devices.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class test
{
private:
    bool is_quiet_ok;
    bool is_quiet_fail;
    bool is_quiet_summary;
    bool is_quiet;
    emulation_devices *device;
    json test_json;

    void print_test_result(string test_name, test_result result, vector<string> errors);
    void print_summary(int ok, int fail, int skip, int total);
    void print_call_stack();

public:
    test(string test_path, emulation_devices *device, bool quiet_ok, bool quiet_fail, bool quiet_summary, bool quiet);
    bool execute();
}; 
