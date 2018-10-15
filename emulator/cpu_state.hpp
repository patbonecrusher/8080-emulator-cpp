#pragma once

#include <stdint.h>
#include <iostream>
 
using namespace std;

typedef struct _condition_codes {    
    _condition_codes ();

    uint8_t    z:1;    
    uint8_t    s:1;    
    uint8_t    p:1;    
    uint8_t    cy:1;    
    uint8_t    ac:1;    
    uint8_t    pad:3;

    friend ostream& operator<<(ostream& os, const _condition_codes& cc);
} condition_codes_t;

typedef struct _cpu_state {
    _cpu_state (uint8_t  *memory);
    ~_cpu_state ();

    // our registers
    uint8_t a, b, c, d, e, h, l;

    uint16_t sp;            // stack pointer
    uint16_t pc;            // program counter
    bool     int_enable;    // irq handling flag

    condition_codes_t cc;

    uint8_t  *memory;  // Might want to use a std::ptr

    uint32_t cycles;

    void logic_flags_a(uint16_t value);
    void flagZSP(uint8_t value);
    void write_mem(uint16_t address, uint8_t value);
    uint8_t read_from_hl();
    void write_to_hl(uint8_t value);

    void arith_flags_a(uint16_t res);
    void do_add_ops(uint16_t value);
    void do_adc_ops(uint16_t value);
    void do_sub_ops(uint16_t value);
    void do_sbb_ops(uint16_t value);
    void do_ana_ops(uint16_t value);
    void do_xra_ops(uint16_t value);
    void do_ora_ops(uint16_t value);
    void do_cmp_ops(uint16_t value);

    void stack_push(uint8_t high, uint8_t low);
    void stack_pop(uint8_t *high, uint8_t *low);

    friend ostream& operator<<(ostream& os, const _cpu_state& cs);
} cpu_state_t;
