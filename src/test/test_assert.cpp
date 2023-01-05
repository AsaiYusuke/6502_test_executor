#include "test/test_assert.hpp"

#include <sstream>

#include "assert/assert_register_value.hpp"
#include "assert/assert_register_read_count.hpp"
#include "assert/assert_register_write_count.hpp"
#include "assert/assert_status_flag_value.hpp"
#include "assert/assert_status_flag_read_count.hpp"
#include "assert/assert_status_flag_write_count.hpp"
#include "assert/assert_memory_value.hpp"
#include "assert/assert_memory_read_count.hpp"
#include "assert/assert_memory_write_count.hpp"
#include "assert/assert_stack_value.hpp"
#include "assert/assert_timeout.hpp"
#include "assert/assert_runtime_error.hpp"
#include "condition/condition_memory.hpp"
#include "util/value_convert.hpp"

test_assert::test_assert(emulation_devices *device, json condition_json)
    : condition(device, condition_json)
{
}

void test_assert::execute()
{
    if (!assert_timeout::test(
            get_device(), get_timeout_def(), &result))
        return;

    for (auto error_def :
         get_device()->get_filtered_errors(
             {runtime_error_type::OUT_OF_SEGMENT,
              runtime_error_type::READONLY_MEMORY,
              runtime_error_type::ILLEGAL_INSTRUCTION,
              runtime_error_type::UNAUTHORIZED_MEMORY,
              runtime_error_type::UNINITIALIZED_MEMORY}))
    {
        assert_runtime_error::test(
            get_device(), error_def, &result);
    }

    for (auto register_def : get_register_defs())
    {
        assert_register_value::test(
            get_device(), register_def, &result);

        assert_register_read_count::test(
            get_device(), register_def, &result);

        assert_register_write_count::test(
            get_device(), register_def, &result);
    }

    for (auto status_flag_def : get_status_flag_defs())
    {
        assert_status_flag_value::test(
            get_device(), status_flag_def, &result);

        assert_status_flag_read_count::test(
            get_device(), status_flag_def, &result);

        assert_status_flag_write_count::test(
            get_device(), status_flag_def, &result);
    }

    for (auto memory_def : get_memory_defs())
    {
        for (auto memory_value_def : memory_def->get_value_sequences())
            assert_memory_value::test(
                get_device(), memory_value_def, &result);

        for (auto memory_read_count_def : memory_def->get_read_counts())
            assert_memory_read_count::test(
                get_device(), memory_read_count_def, &result);

        for (auto memory_write_count_def : memory_def->get_write_counts())
            assert_memory_write_count::test(
                get_device(), memory_write_count_def, &result);
    }

    assert_stack_value::test(get_device(), get_stack_def(), &result);
}

test_result test_assert::get_result(string id)
{
    result.set_id(id);
    return result;
}
