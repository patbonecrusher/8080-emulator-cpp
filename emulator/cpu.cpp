#include <iomanip>

#include "cpu.hpp"

cpu::cpu()
{
    memory =  new uint8_t[0x10000]; 

}

cpu::~cpu() {
    delete[] memory;
}

void cpu::load_firmware_in_memory(uint8_t *mem_buffer, uint16_t address) {
    
}

void cpu::step() {
    uint8_t opcode = memory[cpu_state.pc];    
    cpu_state.pc++;

    this->instructions[opcode].dissassemble(&memory[cpu_state.pc+1], cpu_state.pc);
    cout << cpu_state << endl;

    this->instructions[opcode].execute_fn(&memory[cpu_state.pc+1], cpu_state);
}

void nop0(uint8_t *data, cpu_state_t& state) {
}

void lxib(uint8_t *data, cpu_state_t& state) { 
    state.c = data[0];
    state.b = data[1];
    state.pc += 2;
}

void jmp(uint8_t *data, cpu_state_t& state) { 
    state.pc += 2;
}

void unimpl(uint8_t *data, cpu_state_t& state) {
    cout << "error: unimplement instruction" << endl;
    exit(1);
}


void cpu::load_instruction_set() {
    for (int i=0; i<255; ++i) {
        this->instructions[i] = instruction(i, unimpl, "ERROR", 0);
    }
    this->instructions[0x00] = instruction(0x00, nop0, "NOP", 0);
    this->instructions[0x01] = instruction(0x01, nop0, "LXI    B,#$", 2);
    this->instructions[0xc3] = instruction(0xc3, jmp , "JMP    $", 2);
}