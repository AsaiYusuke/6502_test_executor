#include "test/test_total_result.hpp"

#include "args_parser.hpp"

test_total_result::test_total_result(args_parser *_args)
{
    args = _args;
}

void test_total_result::add_and_print_result(emulation_devices *device, test_result result)
{
    test_result_map[result.get_result_type()]++;

    if (!args->is_quiet() && !args->is_quiet_type(result.get_result_type()))
        result.print_result(device);
}

bool test_total_result::is_success()
{
    return test_result_map[test_result_type::FAIL] == 0;
}

void test_total_result::set_total(int _total)
{
    total = _total;
}

void test_total_result::print_summary()
{
    if (args->is_quiet() || args->is_quiet_summary() || !args->get_test_id().empty())
        return;

    cout
        << "----------------------------------------------------------------------" << endl
        << result_type_name_map[test_result_type::OK] << ": " << test_result_map[test_result_type::OK]
        << ", "
        << result_type_name_map[test_result_type::FAIL] << ": " << test_result_map[test_result_type::FAIL]
        << ", "
        << result_type_name_map[test_result_type::SKIP] << ": " << test_result_map[test_result_type::SKIP]
        << " / TOTAL: " << total << endl;
}
