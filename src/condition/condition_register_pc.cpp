#include "condition/condition_register_pc.hpp"

#include "util/constant.hpp"
#include "util/value_convert.hpp"

using namespace std;

condition_register_pc::condition_register_pc(const emulation_devices *_device, json condition)
{
    start_address = value_convert::get_address(_device, condition["start"]);

    if (condition["testType"].is_null())
        type = test_type::JSR;
    else
        type = test_end_name_type_map[condition["testType"].get<string>()];

    if (condition["end"].is_null())
        end_address = DEFAULT_TEST_RETURN_ADDRESS;
    else
        end_address = value_convert::get_address(_device, condition["end"]);
}

uint16_t condition_register_pc::get_start_address() const
{
    return start_address;
}

test_type condition_register_pc::get_test_type() const
{
    return type;
}

uint16_t condition_register_pc::get_end_address() const
{
    return end_address;
}
