#ifndef __TEST_H_INCLUDED__
#define __TEST_H_INCLUDED__

#include "emulation_devices.h"
#include "../../../json/single_include/nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class test
{
private:
    bool is_quiet;
    vector<string> errors;
    emulation_devices *device;
    json test_json;

    uint16_t target_program_counter;

    enum test_result {
        OK, FAIL, SKIP
    };

    uint8_t to_byte(json value);
    void setup_condition(json condition_json);
    void err(string expected, string actual, string message);
    bool assert_equal(uint8_t expected, uint8_t actual, string message);
    bool assert_equal(bool expected, bool actual, string message);
    bool assert_condition(json condition_json);

public:
    test(string test_path, emulation_devices *device, bool quiet);
    void clear();
    bool execute();
    void print_test_result(test_result result, string test_name);
    void print_summary(int ok, int fail, int skip, int total);
};

#endif
