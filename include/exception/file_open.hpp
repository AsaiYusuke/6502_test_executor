#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class file_open_error : public runtime_error
{
public:
    explicit file_open_error(const string &message)
        : runtime_error("Unable to open file: " + message)
    {
    }
};
