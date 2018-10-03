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

    this->instructions[opcode].dissassemble(&memory[cpu_state.pc+1], cpu_state.pc);
    this->instructions[opcode].execute_fn(&memory[cpu_state.pc+1], cpu_state);
    cout << " " << cpu_state << endl;
}

static int parity(int x, int size)
{
	int i;
	int p = 0;
	x = (x & ((1<<size)-1));
	for (i=0; i<size; i++)
	{
		if (x & 0x1) p++;
		x = x >> 1;
	}
	return (0 == (p & 0x1));
}

static void FlagsZSP(cpu_state_t &state, uint8_t value)
{
    state.cc.z = (value == 0);
    state.cc.s = (0x80 == (value & 0x80));
    state.cc.p = parity(value, 8);    
}

void nop0(uint8_t *data, cpu_state_t& state) {
    state.pc++;
}

void lxib(uint8_t *data, cpu_state_t& state) { 
    state.c = data[0];
    state.b = data[1];
    state.pc += 3;
}

void jmp(uint8_t *data, cpu_state_t& state) {
    state.pc = data[1] << 8 | data[0];
}

void fe_cpi(uint8_t *data, cpu_state_t& state) {
    uint8_t x = state.a - data[0];
    FlagsZSP(state, x);
    state.cc.cy = (state.a < data[0]);
    state.pc++;
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
    this->instructions[0xc3] = instruction(0xc3, jmp , "JMP       $", 2);
    this->instructions[0xfe] = instruction(0xfe, fe_cpi, "CPI      #$", 2);
}