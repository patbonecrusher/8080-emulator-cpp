#include <cstddef>
#include <iostream>
#include <iomanip>

#include "instruction.hpp"

_instruction::_instruction(
    uint8_t id, 
    execute_fn_t execute_fn, 
    std::string const& name,
    uint8_t op_size)
    : id(id)
    , name(name)
    , execute_fn(execute_fn)
    , op_size(op_size) {

}

_instruction::_instruction(_instruction const& copy) {
    this->id = copy.id;
    this->execute_fn = copy.execute_fn;
    this->op_size = copy.op_size;
    this->name = copy.name;
}

uint8_t _instruction::dissassemble(uint8_t *data, uint16_t pc) {
    cout << hex << setw(4) << setfill('0') << std::right << pc << " ";
    cout << hex << setw(4) << setfill('0') << std::right << (int) id << " ";
    cout << setw(11) << setfill(' ') << std::left << name;

    if (op_size == 2) {
        auto b2 = data[1];
        auto b1 = data[0];
        cout << hex << setfill('0');
        cout  << setw(2) << (int) b2 << setw(2) << (int) b1;
    } else if (op_size == 1) {
        auto b1 = data[0];
        cout << hex << setfill('0');
        cout  << setw(2) << (int) b1 << setw(2) << " ";
    } else if (op_size == 0) {
        cout  << setw(4) << setfill(' ') << " ";
    }
    return op_size;
}
