#include <sstream>
#include <fstream>
#include <string>

#include "test/test.h"
#include "test/test_result.h"
#include "test/test_setup.h"
#include "test/test_assert.h"
#include "assert/message.h"
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

    test_scinario = value_convert::parse_all_variable(
        test_scinario["definitions"]["primitives"]["value"], test_scinario);

    device = new emulation_devices(args, test_scinario["config"]);
}

bool test::execute()
{
    test_total_result total_result{device, args, (int)test_scinario["cases"].size()};

    traverse(total_result, test_scinario["cases"], "");

    device->save_coverage();

    total_result.print_summary();

    return total_result.is_success();
}

void test::traverse(test_total_result &total_result, json testcase, string path)
{
    for (auto &element : testcase.items())
    {
        auto id = element.key();
        auto sub_testcase = element.value();

        string test_id_path = path + id;

        if (test_id_path.ends_with("/"))
        {
            traverse(total_result, sub_testcase, test_id_path);
            continue;
        }

        if (!args->get_test_id().empty() && args->get_test_id() != test_id_path)
            continue;

        total_result.add_and_print_result(
            do_test(test_id_path, sub_testcase));
    }
}

test_result test::do_test(string id, json testcase)
{
    if (testcase["skip"].is_boolean() && testcase["skip"].get<bool>())
        return test_result::skip();

    try
    {
        test_setup(
            device,
            value_convert::merge_template(
                test_scinario["definitions"]["templates"]["setup"],
                testcase["setup"]),
            test_scinario["target"])
            .execute();

        device->get_cpu()->execute();

        test_assert assert = test_assert(
            device,
            value_convert::merge_template(
                test_scinario["definitions"]["templates"]["expected"],
                testcase["expected"]));
        assert.execute();

        return assert.get_result(id);
    }
    catch (invalid_argument &e)
    {
        test_result result;
        result.set_id(id);
        result.add_error(message::error_message(e));
        return result;
    }
    catch (cpu_runtime_error &e)
    {
        test_result result;
        result.set_id(id);
        result.add_error(message::error_message(e));
        return result;
    }
}
