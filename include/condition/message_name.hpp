#pragma once

#include <string>

using namespace std;

class i_message_name
{
public:
    i_message_name() {}
    virtual ~i_message_name() {}
    virtual string get_name() const = 0;
};
