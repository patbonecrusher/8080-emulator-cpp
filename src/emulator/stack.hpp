#pragma once

#include <stdint.h>

typedef struct _stack {
    uint16_t reg_SP;
    void pop();
    void push();
} stack_t;