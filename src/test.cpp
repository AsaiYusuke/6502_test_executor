#include <sstream>
#include <fstream>
#include <string>

#include "test.h"
#include "test_result.h"
#include "test_setup.h"
#include "test_assert.h"
#include "util/value_convert.h"
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
    map<test_result_type, int> test_result_map;

    for (auto &element : test_scinario["cases"].items())
    {
        auto id = element.key();
        auto testcase = element.value();

        if (!args->get_test_id().empty() && args->get_test_id() != id)
            continue;

        test_result result;

        try
        {
            result = do_test(testcase);
        }
        catch (exception &e)
        {
            stringstream ss;
            ss << e.what() << endl;
            result.add_error(ss.str());
        }

        test_result_map[result.get_result_type()]++;
        print_test_result(id, result);
    }

    print_summary(test_result_map, test_scinario["cases"].size());

    return test_result_map[test_result_type::FAIL] == 0;
}

test_result test::do_test(json testcase)
{
    if (testcase["skip"].is_boolean() && testcase["skip"].get<bool>())
        return test_result::skip();

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

    return assert.get_result();
}

void test::print_test_result(string test_name, test_result result)
{
    if (args->is_quiet() || args->is_quiet_type(result.get_result_type()))
        return;

    cout
        << result.get_result_name()
        << ": [" << test_name << "]" << endl;

    if (result.get_result_type() == test_result_type::FAIL)
    {
        cerr << endl;
        device->print();

        cerr << endl;
        result.print_error();
    }
}

void test::print_summary(map<test_result_type, int> result_map, int total)
{
    if (args->is_quiet() || args->is_quiet_summary() || !args->get_test_id().empty())
        return;

    cout << "----------------------------------------------------------------------" << endl;
    cout
        << result_type_name_map[test_result_type::OK] << ": " << result_map[test_result_type::OK]
        << ", "
        << result_type_name_map[test_result_type::FAIL] << ": " << result_map[test_result_type::FAIL]
        << ", "
        << result_type_name_map[test_result_type::SKIP] << ": " << result_map[test_result_type::SKIP]
        << " / TOTAL: " << total << endl;
}
