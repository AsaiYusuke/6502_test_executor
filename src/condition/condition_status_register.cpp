#include "condition/condition_status_register.h"

condition_status_register::condition_status_register(json condition)
{
    if (condition.is_null())
        return;

    for (auto &element : condition.items())
    {
        string name = element.key();
        status_flag_type type = status_flag_name_type_map[name];
        bool value = element.value().get<bool>();
        flags.push_back(make_tuple(type, value, name));
    }
}

vector<tuple<status_flag_type, bool, string>> condition_status_register::get_flags()
{
    return flags;
}
