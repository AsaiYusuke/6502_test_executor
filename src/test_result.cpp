#include "test_result.h"

test_result::test_result() : test_result(test_result_type::OK)
{
}

test_result::test_result(test_result_type type)
{
    result_type = type;
}

void test_result::set_id(string _id)
{
    id = _id;
}

void test_result::add_error(string error)
{
    result_type = test_result_type::FAIL;
    errors.push_back(error);
}

test_result_type test_result::get_result_type()
{
    return result_type;
}

string test_result::get_result_name()
{
    return result_type_name_map[result_type];
}

vector<string> test_result::get_errors()
{
    return errors;
}

test_result test_result::skip()
{
    return test_result(test_result_type::SKIP);
}

void test_result::print_result(emulation_devices *device)
{
    cout
        << get_result_name()
        << ": [" << id << "]" << endl;

    if (get_result_type() == test_result_type::FAIL)
    {
        cerr << endl;
        device->print();

        cerr << endl;
        for (string error : errors)
        {
            cerr << error;
        }
    }
}
