#ifndef __MEMORY_ACCESS_H_INCLUDED__
#define __MEMORY_ACCESS_H_INCLUDED__

#include <stdint.h>

class i_memory_access
{
public:
    i_memory_access() {}
    virtual ~i_memory_access() {}
    virtual uint8_t read(uint16_t address) = 0;
    virtual void write(uint16_t address, uint8_t value) = 0;
};

#endif
