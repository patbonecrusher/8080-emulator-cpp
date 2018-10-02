
#pragma once

#include <functional>
#include <iostream>
#include <unordered_map>
#include <string>

#include "cpu_state.hpp"

typedef std::function<void(uint8_t *, cpu_state_t&)> execute_fn_t;

typedef struct _instruction {
    _instruction() {}
    _instruction(
        uint8_t id, 
        execute_fn_t execute_fn, 
        std::string const& name,
        uint8_t op_size = 1);
    _instruction(_instruction const& copy);
    
    uint8_t dissassemble(uint8_t *data, uint16_t pc);

    uint8_t id;
    std::string name;
    execute_fn_t execute_fn;
    uint8_t op_size;
} instruction;

typedef std::unordered_map<uint8_t, instruction>    instruction_map_t;
typedef instruction_map_t::iterator                 instruction_map_it_t;
typedef instruction_map_t::const_iterator           instruction_map_const_it_t;
