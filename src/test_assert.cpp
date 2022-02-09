#include <sstream>

#include "test_assert.h"
#include "condition/address_convert.h"
#include "assert/assert_timeout.h"
#include "assert/assert_register_value.h"
#include "assert/assert_status_flag_value.h"
#include "assert/assert_memory_value.h"
#include "assert/assert_memory_read_count.h"
#include "assert/assert_memory_write_count.h"

test_assert::test_assert(emulation_devices *device, json condition_json)
    : condition(device, condition_json)
{
}

void test_assert::execute()
{
    bool test_result = true;

    test_result &= assert_timeout::test(get_device(), get_timeout_def(), errors);

    if (test_result)
    {
        for (auto register_def : get_register_defs())
            test_result &= assert_register_value::test(
                get_device(), register_def, errors);

        for (auto status_flag_def : get_status_flag_defs())
            test_result &= assert_status_flag_value::test(
                get_device(), status_flag_def, errors);

        for (auto memory_def : get_memory_defs())
        {
            for (auto memory_value_def : memory_def.get_value_sequences())
                test_result &= assert_memory_value::test(
                    get_device(), memory_value_def, errors);

            for (auto memory_read_count_def : memory_def.get_read_counts())
                test_result &= assert_memory_read_count::test(
                    get_device(), memory_read_count_def, errors);

            for (auto memory_write_count_def : memory_def.get_write_counts())
                test_result &= assert_memory_write_count::test(
                    get_device(), memory_write_count_def, errors);
        }
    }

    result = test_result ? test_result::OK : test_result::FAIL;
}

test_result test_assert::get_result()
{
    return result;
}

vector<string> test_assert::get_errors()
{
    return errors;
}
