#pragma once

using namespace std;

class i_memory_access
{
public:
    i_memory_access() {}
    virtual ~i_memory_access() {}
    virtual uint8_t read(uint16_t address) = 0;
    virtual uint8_t read_raw(uint16_t address) = 0;
    virtual uint8_t read_data(uint16_t address) = 0;
    virtual void write(uint16_t address, uint8_t value) = 0;
    virtual void write_data(uint16_t address, uint8_t value) = 0;
};
