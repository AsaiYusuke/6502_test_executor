#include <sstream>
#include <fstream>
#include <string>

#include "test/test.h"
#include "test/test_result.h"
#include "test/test_setup.h"
#include "test/test_assert.h"
#include "assert/message.h"
#include "util/test_detect.h"
#include "util/value_convert.h"
#include "condition/condition.h"
#include "exception/file_open.h"
#include "exception/cpu_runtime_error.h"

json test::read_json(string path)
{
    ifstream in(path);
    if (!in.is_open())
        throw file_open_error(path);

    json test_json;
    in >> test_json;
    in.close();

    return test_json;
}

test::test(args_parser *_args)
{
    args = _args;

    json test_scinario = read_json(args->get_test_path());

    debug = new debug_info(args, test_scinario["config"]);

    if (test_detect::is_project(test_scinario))
        for (auto test_entry : test_scinario["tests"])
        {
            if (test_detect::is_file_reference(test_entry))
            {
                filesystem::path test_path = args->get_test_path();
                test_entry.merge_patch(read_json(test_path.parent_path() / test_entry["file"]));
            }

            test_scinarios.push_back(value_convert::parse_all_variable(
                test_entry["definitions"]["primitives"]["value"], test_entry));
        }
    else
        test_scinarios.push_back(value_convert::parse_all_variable(
            test_scinario["definitions"]["primitives"]["value"], test_scinario));
}

bool test::execute()
{
    test_total_result total_result{device, args};
    int num_cases = 0;

    for (auto test_scinario : test_scinarios)
    {
        num_cases += test_scinario["cases"].size();
        device = new emulation_devices(args, test_scinario["config"], debug);
        traverse(
            total_result,
            test_scinario["target"],
            test_scinario["definitions"]["templates"],
            test_scinario["cases"],
            "");
    }

    total_result.set_total(num_cases);

    debug->save_coverage();

    total_result.print_summary();

    return total_result.is_success();
}

void test::traverse(test_total_result &total_result, json test_target, json test_template, json test_case, string path)
{
    for (auto &element : test_case.items())
    {
        auto id = element.key();
        auto sub_test_case = element.value();

        string test_id_path = path + id;

        if (test_id_path.ends_with("/"))
        {
            traverse(total_result, test_target, test_template, sub_test_case, test_id_path);
            continue;
        }

        if (!args->get_test_id().empty() && args->get_test_id() != test_id_path)
            continue;

        total_result.add_and_print_result(
            do_test(test_id_path, test_target, test_template, sub_test_case));
    }
}

test_result test::do_test(string id, json test_target, json test_template, json test_case)
{
    if (test_case["skip"].is_boolean() && test_case["skip"].get<bool>())
        return test_result::skip();

    try
    {
        test_setup(
            device,
            value_convert::merge_template(
                test_template["setup"],
                test_case["setup"]),
            test_target)
            .execute();

        device->get_cpu()->execute();

        test_assert assert = test_assert(
            device,
            value_convert::merge_template(
                test_template["expected"],
                test_case["expected"]));
        assert.execute();

        return assert.get_result(id);
    }
    catch (invalid_argument &e)
    {
        test_result result;
        result.set_id(id);
        result.add_error(message::error_message(e));
        result.set_device_not_ready();
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
