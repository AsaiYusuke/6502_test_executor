#pragma once

using namespace std;

class parse_abort
{
private:
    int return_value;

public:
    parse_abort(int _return_value)
    {
        return_value = _return_value;
    }

    int get_return_value()
    {
        return return_value;
    }
};
