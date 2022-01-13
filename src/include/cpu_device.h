#ifndef __CPU_DEVICE_H_INCLUDED__
#define __CPU_DEVICE_H_INCLUDED__

#include "mos6502.h"
#include "memory_access.h"

using namespace std;

#define DEFAULT_TIMEOUT 100000

class cpu_device
{
private:
    mos6502 *cpu;

public:
    cpu_device(i_memory_access *memory_access);
    void clear(uint16_t target_program_counter);
    bool execute();
    int timeout();

    uint16_t get_reg_pc();
    void set_reg_pc(uint16_t value);
    uint8_t get_reg_a();
    void set_reg_a(uint8_t value);
    uint8_t get_reg_x();
    void set_reg_x(uint8_t value);
    uint8_t get_reg_y();
    void set_reg_y(uint8_t value);
    bool is_reg_status_negative_flag();
    void set_reg_status_negative_flag(bool value);
    bool is_reg_status_overflow_flag();
    void set_reg_status_overflow_flag(bool value);
    bool is_reg_status_break_flag();
    void set_reg_status_break_flag(bool value);
    bool is_reg_status_decimal_flag();
    void set_reg_status_decimal_flag(bool value);
    bool is_reg_status_interrupt_flag();
    void set_reg_status_interrupt_flag(bool value);
    bool is_reg_status_zero_flag();
    void set_reg_status_zero_flag(bool value);
    bool is_reg_status_carry_flag();
    void set_reg_status_carry_flag(bool value);

    void print();
};

#endif
