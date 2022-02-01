#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class timeout_error : public runtime_error
{
public:
    timeout_error(const string &message) : runtime_error("Timeout error: " + message) {}
};
