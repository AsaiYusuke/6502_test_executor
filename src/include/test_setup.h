#pragma once

#include "condition/condition.h"

using namespace std;

class test_setup
{
private:
    condition *cond;

public:
    test_setup(condition *cond);
    bool execute();
};
