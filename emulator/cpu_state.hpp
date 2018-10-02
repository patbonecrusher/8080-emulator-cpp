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
    _cpu_state ();
    ~_cpu_state ();

    // our registers
    uint8_t a, b, c, d, e, h, l;

    uint16_t sp;            // stack pointer
    uint16_t pc;            // program counter
    bool     int_enable;    // irq handling flag

    condition_codes_t cc;

    friend ostream& operator<<(ostream& os, const _cpu_state& cs);
} cpu_state_t;