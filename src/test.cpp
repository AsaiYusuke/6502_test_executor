#include <sstream>
#include <fstream>
#include <string>

#include "test.h"
#include "test_setup.h"
#include "test_assert.h"
#include "util/address_convert.h"
#include "condition/condition.h"

test::test(args_parser *_args)
{
    args = _args;

    ifstream in(args->get_test_path());
    in >> test_scinario;

    device = new emulation_devices(args, test_scinario["config"]);
}

bool test::execute()
{
    map<test_result, int> test_result_map;

    for (auto &element : test_scinario["cases"].items())
    {
        auto name = element.key();
        auto testcase = element.value();

        if (testcase["skip"].is_boolean() && testcase["skip"].get<bool>())
        {
            print_test_result(
                name,
                test_result::SKIP,
                {});
            test_result_map[test_result::SKIP]++;
            continue;
        }

        try
        {
            test_setup(
                device,
                testcase["setup"],
                test_scinario["target"])
                .execute();

            device->get_cpu()->execute();

            test_assert assert = test_assert(
                device,
                testcase["expected"]);
            assert.execute();

            print_test_result(
                name,
                assert.get_result(),
                assert.get_errors());

            auto result = assert.get_result() == test_result::FAIL_WITH_CALLSTACK
                              ? test_result::FAIL
                              : assert.get_result();
            test_result_map[result]++;
        }
        catch (exception &e)
        {
            print_test_result(
                name,
                test_result::FAIL,
                {e.what()});

            test_result_map[test_result::FAIL]++;
        }
    }

    print_summary(
        test_result_map[test_result::OK],
        test_result_map[test_result::FAIL],
        test_result_map[test_result::SKIP],
        test_scinario["cases"].size());

    return test_result_map[test_result::FAIL] == 0;
}

void test::print_test_result(string test_name, test_result result, vector<string> errors)
{
    if (args->is_quiet())
        return;

    switch (result)
    {
    case test_result::OK:
        if (args->is_quiet_success())
            return;
        cout << "OK";
        break;
    case test_result::FAIL:
    case test_result::FAIL_WITH_CALLSTACK:
        if (args->is_quiet_failed())
            return;
        cout << "FAIL";
        break;
    case test_result::SKIP:
        if (args->is_quiet_success())
            return;
        cout << "SKIP";
        break;
    }

    cout << ": [" << test_name << "]" << endl;

    if (result == test_result::FAIL || result == test_result::FAIL_WITH_CALLSTACK)
    {
        cerr << endl;
        device->print();
    }

    if (!errors.empty())
    {
        cerr << endl;
        for (string error : errors)
        {
            cerr << error << endl;
        }
        cerr << endl;
    }

    if (result == test_result::FAIL_WITH_CALLSTACK)
    {
        print_call_stack();
    }
}

void test::print_summary(int ok, int fail, int skip, int total)
{
    if (args->is_quiet() || args->is_quiet_summary())
        return;

    cout << "----------------------------------------------------------------------" << endl;
    cout << "OK: " << ok;
    cout << ", FAIL: " << fail;
    cout << ", SKIP: " << skip;
    cout << " / TOTAL: " << total << endl;
}

void test::print_call_stack()
{
    cerr << "Call stack:" << endl;

    int index = 0;
    for (auto address : device->get_cpu()->get_call_stack())
    {
        if (address == 0xFFFF)
            continue;

        cerr << index++ << ":  " << address_convert::to_hex_string(address) << " : ";
        cerr << device->get_memory()->get_source_line(address) << endl;
    }
    cerr << endl;
}
