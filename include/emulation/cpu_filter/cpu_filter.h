#pragma once

using namespace std;

class cpu_device;

class i_cpu_filter
{
public:
    i_cpu_filter() {}
    virtual ~i_cpu_filter() {}
    virtual void clear() = 0;
    virtual bool pre() = 0;
    virtual bool post() = 0;
};
