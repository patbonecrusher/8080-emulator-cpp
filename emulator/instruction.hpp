
#pragma once

#include <functional>
#include <iostream>
#include <unordered_map>
#include <string>

#include "cpu_state.hpp"


typedef struct _op_info {
    uint8_t code;
    uint16_t pc;
    uint16_t next_pc;
    uint8_t cycle_count;
    union {
        uint8_t data[2];
        uint8_t val_1_byte;
        uint16_t val_2_bytes;
    };
} op_info_t;

typedef std::function<void(uint8_t*, op_info_t&, cpu_state_t&)> execute_fn_t;

typedef struct _instruction {
    _instruction() {}
    _instruction(
        uint8_t id, 
        std::string const& name,
        uint8_t cycle_count,
        uint8_t op_size = 1,
        execute_fn_t execute_fn = NULL);
    _instruction(_instruction const& copy);
    
    uint8_t dissassemble(uint8_t *data, uint16_t pc);
    uint8_t execute(uint8_t * data, cpu_state_t &state);

    uint8_t id;
    std::string name;

    execute_fn_t execute_fn;
    
    uint8_t op_size;
    uint8_t cycle_count;
} instruction;

typedef std::unordered_map<uint8_t, instruction>    instruction_map_t;
typedef instruction_map_t::iterator                 instruction_map_it_t;
typedef instruction_map_t::const_iterator           instruction_map_const_it_t;
