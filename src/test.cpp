#include <sstream>
#include <fstream>
#include <string>

#include "test.h"
#include "test_setup.h"
#include "test_assert.h"
#include "condition/condition.h"

test::test(string test_path, emulation_devices *device, bool quiet_ok, bool quiet_fail, bool quiet_summary, bool quiet)
{
    ifstream in(test_path);
    in >> test_json;

    this->device = device;

    is_quiet_ok = quiet_ok;
    is_quiet_fail = quiet_fail;
    is_quiet_summary = quiet_summary;
    is_quiet = quiet;
}

bool test::execute()
{
    map<test_result, int> test_result_map;

    for (auto &element : test_json["cases"].items())
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
                new condition(
                    device,
                    testcase["setup"],
                    test_json["target"]))
                .execute();

            device->get_cpu()->execute();

            test_assert assert = test_assert(
                new condition(
                    device,
                    testcase["expected"]));
            assert.execute();

            print_test_result(
                name,
                assert.get_result(),
                assert.get_errors());

            test_result_map[assert.get_result()]++;
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
        test_json["cases"].size());

    return test_result_map[test_result::FAIL] == 0;
}

void test::print_test_result(string test_name, test_result result, vector<string> errors)
{
    if (is_quiet)
        return;

    switch (result)
    {
    case test_result::OK:
        if (is_quiet_ok)
            return;
        cout << "OK";
        break;
    case test_result::FAIL:
        if (is_quiet_fail)
            return;
        cout << "FAIL";
        break;
    case test_result::SKIP:
        if (is_quiet_ok)
            return;
        cout << "SKIP";
        break;
    }

    cout << ": [" << test_name << "]" << endl;

    if (result == test_result::FAIL)
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
}

void test::print_summary(int ok, int fail, int skip, int total)
{
    if (is_quiet || is_quiet_summary)
        return;

    cout << "----------------------------------------------------------------------" << endl;
    cout << "OK: " << ok;
    cout << ", FAIL: " << fail;
    cout << ", SKIP: " << skip;
    cout << " / TOTAL: " << total << endl;
}