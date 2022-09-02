#include <sstream>
#include <fstream>
#include <string>

#include "test/test.h"
#include "test/test_total_result.h"
#include "test/test_result.h"
#include "test/test_setup.h"
#include "test/test_assert.h"
#include "util/value_convert.h"
#include "condition/condition.h"
#include "exception/file_open.h"
#include "exception/cpu_runtime_error.h"

test::test(args_parser *_args)
{
    args = _args;

    ifstream in(args->get_test_path());
    if (!in.is_open())
        throw file_open_error(args->get_test_path());
    in >> test_scinario;
    in.close();

    device = new emulation_devices(args, test_scinario["config"]);
}

bool test::execute()
{
    test_total_result total_result{device, args, (int)test_scinario["cases"].size()};

    for (auto &element : test_scinario["cases"].items())
    {
        auto id = element.key();
        auto testcase = element.value();

        if (!args->get_test_id().empty() && args->get_test_id() != id)
            continue;

        total_result.add_and_print_result(
            do_test(id, testcase));
    }

    total_result.print_summary();

    return total_result.is_success();
}

test_result test::do_test(string id, json testcase)
{
    if (testcase["skip"].is_boolean() && testcase["skip"].get<bool>())
        return test_result::skip();

    try
    {
        test_setup(
            device,
            test_scinario["definitions"]["setup"],
            testcase["setup"],
            test_scinario["target"])
            .execute();

        device->get_cpu()->execute();

        test_assert assert = test_assert(
            device,
            test_scinario["definitions"]["expected"],
            testcase["expected"]);
        assert.execute();

        return assert.get_result(id);
    }
    catch (invalid_argument &e)
    {
        test_result result;
        result.set_id(id);
        stringstream ss;
        ss << e.what() << endl;
        ss << endl;
        result.add_error(ss.str());
        return result;
    }
    catch (cpu_runtime_error &e)
    {
        test_result result;
        result.set_id(id);
        stringstream ss;
        ss << e.what() << endl;
        ss << endl;
        result.add_error(ss.str());
        return result;
    }
}
