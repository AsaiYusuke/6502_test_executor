#include "condition/condition_timeout.h"
#include "util/expression_executer.h"

condition_timeout::condition_timeout(json condition)
{
    if (expression_executer::find(condition))
        for (auto &expression : expression_executer::get(condition))
            expressions.push_back(
                make_pair(
                    expression.first,
                    expression.second.get<bool>()));
    else
        expressions.push_back(
            make_pair(
                operator_type::EQ,
                false));
}

vector<pair<operator_type, bool>> condition_timeout::get_expressions()
{
    return expressions;
}
