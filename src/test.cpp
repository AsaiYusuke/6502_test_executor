#include <sstream>
#include <fstream>
#include <string>

#include "test.h"

using namespace std;

test::test(string test_path, emulation_devices *device, bool quiet_ok, bool quiet_fail, bool quiet_summary, bool quiet)
{
    ifstream in(test_path);
    in >> test_json;

    this->device = device;

    if (test_json["target"].contains("address"))
        target_program_counter =
            device->to_byte(
                test_json["target"]["address"].get<string>());
    else
        target_program_counter =
            device->get_address(
                test_json["target"]["label"].get<string>());

    is_quiet_ok = quiet_ok;
    is_quiet_fail = quiet_fail;
    is_quiet_summary = quiet_summary;
    is_quiet = quiet;
}

uint8_t test::to_byte(json value)
{
    if (value.is_string())
        return device->to_byte(
            value.get<string>());

    return value.get<uint8_t>();
}

void test::setup_condition(json condition)
{
    auto cpu = device->get_cpu();

    if (!condition["A"].is_null())
        cpu->set_reg_a(to_byte(condition["A"]));

    if (!condition["X"].is_null())
        cpu->set_reg_a(to_byte(condition["X"]));

    if (!condition["Y"].is_null())
        cpu->set_reg_a(to_byte(condition["Y"]));

    if (!condition["Status"].is_null())
    {
        if (!condition["Status"]["Negative"].is_null())
            cpu->set_reg_status_negative_flag(
                condition["Status"]["Negative"].get<bool>());

        if (!condition["Status"]["Overflow"].is_null())
            cpu->set_reg_status_overflow_flag(
                condition["Status"]["Overflow"].get<bool>());

        if (!condition["Status"]["Break"].is_null())
            cpu->set_reg_status_break_flag(
                condition["Status"]["Break"].get<bool>());

        if (!condition["Status"]["Decimal"].is_null())
            cpu->set_reg_status_decimal_flag(
                condition["Status"]["Decimal"].get<bool>());

        if (!condition["Status"]["Interrupt"].is_null())
            cpu->set_status_interrupt_flag(
                condition["Status"]["Interrupt"].get<bool>());

        if (!condition["Status"]["Zero"].is_null())
            cpu->set_status_zero_flag(
                condition["Status"]["Zero"].get<bool>());

        if (!condition["Status"]["Carry"].is_null())
            cpu->set_status_carry_flag(
                condition["Status"]["Carry"].get<bool>());
    }

    if (!condition["memory"].is_null())
    {
        auto memory = device->get_memory();

        for (auto &memory_def : condition["memory"])
        {
            uint16_t address;
            if (memory_def.contains("address"))
                address = device->to_byte(
                    memory_def["address"].get<string>());
            else
                address = device->get_address(
                    memory_def["label"].get<string>());

            uint8_t value;
            if (memory_def["value"].is_string())
                value = device->to_byte(
                    memory_def["value"].get<string>());
            else
                value = device->two_complement_byte(
                    memory_def["value"].get<uint8_t>());

            memory->write(address, value);
        }
    }
}

void test::err(string expected, string actual, string message)
{
    errors.push_back(message + ": [Expected: " + expected + ", Actual: " + actual + "]");
}

bool test::assert_equal(uint8_t expected, uint8_t actual, string message)
{
    if (expected != actual)
    {
        err(to_string(expected), to_string(actual), message);
        return false;
    }
    return true;
}

bool test::assert_equal(bool expected, bool actual, string message)
{
    if (expected != actual)
    {
        err((expected ? "True" : "False"), (actual ? "True" : "False"), message);
        return false;
    }
    return true;
}

bool test::assert_condition(json condition)
{
    bool result = true;

    auto cpu = device->get_cpu();

    if (!condition["A"].is_null())
        result &= assert_equal(to_byte(condition["A"]), cpu->get_reg_a(), "Register A");
    if (!condition["X"].is_null())
        result &= assert_equal(to_byte(condition["X"]), cpu->get_reg_x(), "Register X");
    if (!condition["Y"].is_null())
        result &= assert_equal(to_byte(condition["Y"]), cpu->get_reg_y(), "Register Y");

    if (!condition["Status"]["Negative"].is_null())
        result &= assert_equal(
            condition["Status"]["Negative"].get<bool>(),
            cpu->is_reg_status_negative_flag(),
            "Register Status(Negative flag)");

    if (!condition["Status"]["Overflow"].is_null())
        result &= assert_equal(
            condition["Status"]["Overflow"].get<bool>(),
            cpu->is_reg_status_overflow_flag(),
            "Register Status(Overflow flag)");

    if (!condition["Status"]["Break"].is_null())
        result &= assert_equal(
            condition["Status"]["Break"].get<bool>(),
            cpu->is_reg_status_break_flag(),
            "Register Status(Break flag)");

    if (!condition["Status"]["Decimal"].is_null())
        result &= assert_equal(
            condition["Status"]["Decimal"].get<bool>(),
            cpu->is_reg_status_decimal_flag(),
            "Register Status(Decimal flag)");

    if (!condition["Status"]["Interrupt"].is_null())
        result &= assert_equal(
            condition["Status"]["Interrupt"].get<bool>(),
            cpu->is_reg_status_interrupt_flag(),
            "Register Status(Interrupt flag)");

    if (!condition["Status"]["Zero"].is_null())
        result &= assert_equal(
            condition["Status"]["Zero"].get<bool>(),
            cpu->is_reg_status_zero_flag(),
            "Register Status(Zero flag)");

    if (!condition["Status"]["Carry"].is_null())
        result &= assert_equal(
            condition["Status"]["Carry"].get<bool>(),
            cpu->is_reg_status_carry_flag(),
            "Register Status(Carry flag)");

    auto memory = device->get_memory();

    for (auto &memory_def : condition["memory"])
    {
        string address_name;
        uint16_t address;
        if (memory_def.contains("address"))
        {
            address = device->to_byte(
                memory_def["address"].get<string>());
            stringstream ss;
            ss << hex << address;
            address_name =
                "Memory " + memory_def["address"].get<string>() + "($" + ss.str() + ")";
        }
        else
        {
            address = device->get_address(
                memory_def["label"].get<string>());
            stringstream ss;
            ss << hex << address;
            address_name =
                "Memory " + memory_def["label"].get<string>() + "($" + ss.str() + ")";
        }

        uint8_t value;
        if (memory_def["value"].is_string())
            value = device->to_byte(
                memory_def["value"].get<string>());
        else
            value = device->two_complement_byte(
                memory_def["value"].get<uint8_t>());

        result &= assert_equal(
            value, memory->read(address), address_name);
    }
    return result;
}

void test::clear()
{
    device->clear(target_program_counter);
    errors.clear();
}

bool test::execute()
{
    bool return_value = true;
    map<test_result, int> test_result_map;

    for (json &testcase : test_json["cases"])
    {
        if (testcase["skip"].is_boolean() && testcase["skip"].get<bool>())
        {
            print_test_result(SKIP, testcase["name"].get<string>());
            test_result_map[SKIP]++;
            continue;
        }

        bool result = true;

        clear();

        setup_condition(testcase["setup"]);

        result = device->get_cpu()->execute();

        assert_equal(true, result, "Execution time < " + to_string(device->get_cpu()->timeout()));
        if (result)
            result = assert_condition(testcase["expected"]);

        print_test_result(result ? OK : FAIL, testcase["name"].get<string>());
        test_result_map[result ? OK : FAIL]++;

        return_value &= result;
    }

    print_summary(test_result_map[OK], test_result_map[FAIL], test_result_map[SKIP], test_json["cases"].size());

    return return_value;
}

void test::print_test_result(test_result result, string test_name)
{
    if (is_quiet)
        return;

    switch (result)
    {
    case OK:
        if (is_quiet_ok)
            return;
        cout << "OK";
        break;
    case FAIL:
        if (is_quiet_fail)
            return;
        cout << "FAIL";
        break;
    case SKIP:
        if (is_quiet_ok)
            return;
        cout << "SKIP";
        break;
    }

    cout << ": [" << test_name << "]" << endl;

    if (result == FAIL)
        device->print();

    if (!errors.empty())
    {
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