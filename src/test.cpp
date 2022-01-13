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

    json target = test_json["target"];
    if (target.contains("address"))
        target_program_counter = to_byte(target["address"]);
    else
        target_program_counter = get_address(target);

    is_quiet_ok = quiet_ok;
    is_quiet_fail = quiet_fail;
    is_quiet_summary = quiet_summary;
    is_quiet = quiet;
}

uint16_t test::get_address(json value)
{
    return device->get_address(
        value["label"].get<string>(),
        to_byte(value["offset"]));
}

uint8_t test::to_byte(json value)
{
    if (value.is_null())
        return 0;

    if (value.is_string())
        return device->to_byte(value.get<string>());

    return device->two_complement_byte(value.get<uint8_t>());
}

void test::setup_condition(json condition)
{
    auto cpu = device->get_cpu();

    cpu->set_reg_a(to_byte(condition["A"]));
    cpu->set_reg_x(to_byte(condition["X"]));
    cpu->set_reg_y(to_byte(condition["Y"]));
    if (!condition["Status"].is_null())
    {
        cpu->set_reg_status_negative_flag(condition["Status"].value("Negative", false));
        cpu->set_reg_status_overflow_flag(condition["Status"].value("Overflow", false));
        cpu->set_reg_status_break_flag(condition["Status"].value("Break", false));
        cpu->set_reg_status_decimal_flag(condition["Status"].value("Decimal", false));
        cpu->set_reg_status_interrupt_flag(condition["Status"].value("Interrupt", false));
        cpu->set_reg_status_zero_flag(condition["Status"].value("Zero", false));
        cpu->set_reg_status_carry_flag(condition["Status"].value("Carry", false));
    }

    if (!condition["memory"].is_null())
    {
        auto memory = device->get_memory();

        for (auto &memory_def : condition["memory"])
        {
            uint16_t address;
            if (memory_def.contains("address"))
                address = to_byte(memory_def["address"]);
            else
                address = get_address(memory_def);

            uint8_t value = to_byte(memory_def["value"]);

            memory->write(address, value);
        }
    }
}

string test::to_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << value;
    return ss.str();
}

void test::err(string expected, string actual, string message)
{
    errors.push_back(message + ": [Expected: " + expected + ", Actual: " + actual + "]");
}

string test::to_string(bool value)
{
    return value ? "True" : "False";
}

template <typename T> bool test::assert_equal(T expected, T actual, string message)
{
    if (expected != actual)
    {
        err(to_string(expected), to_string(actual), message);
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
            address = to_byte(memory_def["address"]);
            address_name =
                "Memory " + memory_def["address"].get<string>();
        }
        else
        {
            address = get_address(memory_def);
            stringstream ss;
            ss << "Memory " << memory_def["label"].get<string>();
            ss << " + " << to_hex_string(to_byte(memory_def["offset"]));
            ss << " (" << to_hex_string(address) << ")";
            address_name = ss.str();
        }

        uint8_t value = to_byte(memory_def["value"]);

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