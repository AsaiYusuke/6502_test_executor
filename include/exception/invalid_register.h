#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class invalid_register_error : public invalid_argument
{
public:
    invalid_register_error(const string &message) : invalid_argument("Invalid register: " + message) {}
};
