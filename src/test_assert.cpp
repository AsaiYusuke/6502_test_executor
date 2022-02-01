#include <sstream>

#include "test_assert.h"
#include "condition/address_convert.h"

test_assert::test_assert(condition *cond)
{
    this->cond = cond;
}

void test_assert::err(string expected, string actual, string message)
{
    errors.push_back(message + ": Expected: " + expected + ", Actual: " + actual);
}

string to_string(vector<uint8_t> values)
{
    stringstream ss;
    ss << "[";

    ss << address_convert::to_hex_string(values[0]);
    for (decltype(values.size()) index = 1, size = values.size(); index < size; index++) {
      ss << ", " << address_convert::to_hex_string(values[index]);
    }

    ss << "]";
    return ss.str();
}

string to_string(bool value)
{
    return value ? "True" : "False";
}

template <typename T>
bool test_assert::assert_equal(T expected, T actual, string message)
{
    if (expected != actual)
    {
        err(to_string(expected), to_string(actual), message);
        return false;
    }
    return true;
}

void test_assert::execute()
{
    bool test_result = true;

    cpu_device *cpu_dev = cond->get_device()->get_cpu();
    for (auto register_def : cond->get_register_defs())
    {
        if (register_def.is_blank())
            continue;

        test_result &= assert_equal(
            register_def.get_value(),
            cpu_dev->get_register(register_def.get_type()),
            "Register [" + string(register_type_name_map[register_def.get_type()]) + "]");
    }

    uint8_t status = cpu_dev->get_register(register_type::Status);
    for (auto status_flag_def : cond->get_status_flag_defs())
    {
        test_result &= assert_equal(
            get<1>(status_flag_def),
            (status & (uint8_t)get<0>(status_flag_def)) > 0,
            "Register [" + string(register_type_name_map[register_type::Status]) + " (" + get<2>(status_flag_def) + ")]");
    }

    memory_device *mem_dev = cond->get_device()->get_memory();

    for (auto memory_def : cond->get_memory_value_defs())
    {
        auto address = get<0>(memory_def);
        auto history = get<1>(memory_def);
        auto name = get<2>(memory_def);
        test_result &= assert_equal(
            history,
            mem_dev->get_write_sequence(address, history.size()),
            "Memory data [" + name + "]");
    }

    for (auto count_def : cond->get_memory_read_count_defs())
    {
        auto address = get<0>(count_def);
        auto count = get<1>(count_def);
        auto name = get<2>(count_def);
        test_result &= assert_equal(
            count,
            mem_dev->get_read_count(address),
            "Memory read count [" + name + "]");
    }

    for (auto count_def : cond->get_memory_write_count_defs())
    {
        auto address = get<0>(count_def);
        auto count = get<1>(count_def);
        auto name = get<2>(count_def);
        test_result &= assert_equal(
            count,
            mem_dev->get_write_count(address),
            "Memory write count [" + name + "]");
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
