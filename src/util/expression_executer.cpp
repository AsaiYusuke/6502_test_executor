#include "util/expression_executer.h"

bool expression_executer::find(json value)
{
    if (!value.is_object())
        return false;

    for (auto &element : value.items())
        if (operator_name_type_map.count(element.key()) == 0)
            return false;

    return true;
}

vector<pair<operator_type, json>> expression_executer::get(json value)
{
    vector<pair<operator_type, json>> result;
    for (auto &element : value.items())
        result.push_back(
            make_pair(
                operator_name_type_map[element.key()],
                element.value()));

    return result;
}
