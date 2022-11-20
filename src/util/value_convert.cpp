#include <sstream>
#include "enum/value_type.h"
#include "util/value_convert.h"
#include "emulation/emulation_devices.h"
#include "exception/parse_abort.h"

uint16_t value_convert::get_address(emulation_devices *device, json value)
{
    uint16_t address;
    if (value.is_null())
        return 0;

    if (!value["address"].is_null())
        return parse_json_number(device, value["address"]);

    if (!value["label"].is_null())
        return device->get_address(value["label"].get<string>()) +
               parse_json_number(device, value["offset"]);
    
    return 0;
}

uint16_t value_convert::parse_json_number(emulation_devices *device, json value)
{
    switch (value.type())
    {
    case json::value_t::number_integer:
    case json::value_t::number_unsigned:
        return value.get<uint16_t>();
    case json::value_t::string:
    {
        string str = value.get<string>();
        if (str.compare(0, 1, "$") == 0)
            return stoi(str.substr(1), 0, 16);
        else if (str.compare(0, 1, "%") == 0)
            return stoi(str.substr(1), 0, 2);
        break;
    }
    case json::value_t::object:
    {
        auto address = get_address(device, value);
        if (value["type"].is_null())
            return device->get_memory()->read(address);

        if (value["type"].is_string())
            switch (value_name_type_map[value["type"].get<string>()])
            {
            case value_type::VALUE:
                return device->get_memory()->read(address);
            case value_type::HIBYTE:
                return address >> 8;
            case value_type::LOBYTE:
                return address & 0xFF;
            case value_type::RTS_HIBYTE:
                return (address - 1) >> 8;
            case value_type::RTS_LOBYTE:
                return (address - 1) & 0xFF;
            }
        break;
    }
    case json::value_t::null:
        return 0;
    }

    throw invalid_argument("Invalid format: " + to_string(value));
}

uint16_t value_convert::to_two_complement_byte(emulation_devices *device, json value)
{
    auto number = parse_json_number(device, value);
    if (number < 0)
        number += 256;
    return number;
}

string value_convert::to_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << value;
    return ss.str();
}

string value_convert::to_zero_filled_hex_string(uint16_t value)
{
    stringstream ss;
    ss << "$" << uppercase << hex << setw(4) << setfill('0') << value;
    return ss.str();
}

json value_convert::parse_variable(json definitions_def, json value)
{
    if (value.is_string() && definitions_def.is_object())
    {
        auto valueStr = value.get<string>();
        if (valueStr.front() == '@')
        {
            if (definitions_def.contains(valueStr.substr(1)))
                return definitions_def[valueStr.substr(1)];

            throw invalid_argument("Definition not found. (key: " + valueStr.substr(1) + ", definitions: " + to_string(definitions_def) + ")");
        }
    }
    return value;
}

json value_convert::parse_template(json definitions_def, json value)
{
    auto result = value_convert::parse_variable(definitions_def, value);
    if (!result.is_object())
        throw invalid_argument("Definition not found: " + to_string(value));

    return result;
}

json value_convert::parse_all_variable(json definitions_def, json condition)
{
    if (condition.is_array())
        for (auto &entity : condition)
            entity = parse_all_variable(definitions_def, entity);
    if (condition.is_object())
        for (auto &entity : condition.items())
            if (entity.key() != "template")
                condition[entity.key()] = parse_all_variable(definitions_def, entity.value());
    if (condition.is_string())
        condition = parse_variable(definitions_def, condition);

    return condition;
}

json value_convert::merge_template(json templates, json testcase)
{
    if (testcase["template"].is_array())
        for (auto &template_ref : testcase["template"])
        {
            auto template_def = value_convert::parse_template(templates, template_ref);

            auto testcase_memory_def = testcase["memory"];
            testcase.merge_patch(template_def);

            if (template_def["memory"].is_array())
                for (auto &memory_def : template_def["memory"])
                    testcase_memory_def.push_back(memory_def);
            testcase["memory"] = testcase_memory_def;
        }

    if (testcase["mockedProc"].is_array())
        for (decltype(testcase["mockedProc"].size()) proc_offset = 0, proc_size = testcase["mockedProc"].size(); proc_offset < proc_size; proc_offset++)
        {
            auto mocked_proc_def = testcase["mockedProc"][proc_offset]["setValue"];
            for (decltype(mocked_proc_def.size()) value_offset = 0, value_size = mocked_proc_def.size(); value_offset < value_size; value_offset++)
                testcase["mockedProc"][proc_offset]["setValue"][value_offset] =
                    merge_template(templates, mocked_proc_def[value_offset]);
        }

    return testcase;
}
