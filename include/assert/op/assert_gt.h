#pragma once

using namespace std;

class assert_gt
{
public:
    template <typename T>
    static bool test(T a, T b)
    {
        return a > b;
    }
};
