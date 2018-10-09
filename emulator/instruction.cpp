#include <cstddef>
#include <iostream>
#include <iomanip>

#include "instruction.hpp"

_instruction::_instruction(
    uint8_t id, 
    std::string const& name,
    uint8_t cycle_count,
    uint8_t op_size,
    execute_fn_t execute_fn)
    : id(id)
    , name(name)
    , cycle_count(cycle_count)
    , op_size(op_size) 
    , execute_fn(execute_fn)
    {

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

    if (op_size == 3) {
        auto b2 = data[1];
        auto b1 = data[0];
        cout << hex << setfill('0') << std::right;
        cout  << setw(2) << (int) b2 << setw(2) << (int) b1;
    } else if (op_size == 2) {
        auto b1 = data[0];
        cout << hex << setfill('0') << std::right;
        cout  << setw(2) << (int) b1 << setfill(' ') << setw(2) << " ";
    } else if (op_size == 1) {
        cout  << setw(4) << setfill(' ') << " ";
    }
    return op_size;
}

uint8_t _instruction::execute(uint8_t * data, cpu_state_t &state) {
    // Increment the pc automatically.  If an instruction changes it, that
    // is fine.
    state.pc += this->op_size;

    // uint16_t value = 0;
    // if      (op_size == 3) { value = data[1] << 8 | data[0]; }
    // else if (op_size == 2) { value = data[0]; }

    // Call the fn if one exists, the instruction could be a no-op.
    if (this->execute_fn != NULL) this->execute_fn(data, state);
    
    state.cycles += this->cycle_count;

    // Return the cycle_count.  Even if some instruction could return 1 of
    // two values, we don't really care in our emulation.  We will pick
    // the biggest.
    return this->cycle_count;
}
