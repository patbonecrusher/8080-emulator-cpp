// g++ -std=gnu++17 emu8080.cpp -o emu8080
// ./emu8080 roms/file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <cstddef>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "cpu_state.hpp"
#include "cpu.hpp"

using namespace std;
using std::cout;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;
using std::byte;

typedef vector<byte> bin_array;
typedef bin_array::iterator bin_array_it;
typedef bin_array::const_iterator bin_array_const_it;

int Disassemble8080Op(unsigned char *codebuffer, int pc)
{
	unsigned char *code = &codebuffer[pc];
	int opbytes = 1;
	printf("%04x ", pc);
	switch (*code)
	{
		case 0x00: printf("NOP         "); break;
		case 0x01: printf("LXI    B,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x02: printf("STAX   B    "); break;
		case 0x03: printf("INX    B    "); break;
		case 0x04: printf("INR    B"); break;
		case 0x05: printf("DCR    B"); break;
		case 0x06: printf("MVI    B,#$%02x", code[1]); opbytes=2; break;
		case 0x07: printf("RLC"); break;
		case 0x08: printf("NOP"); break;
		case 0x09: printf("DAD    B"); break;
		case 0x0a: printf("LDAX   B"); break;
		case 0x0b: printf("DCX    B"); break;
		case 0x0c: printf("INR    C"); break;
		case 0x0d: printf("DCR    C"); break;
		case 0x0e: printf("MVI    C,#$%02x", code[1]); opbytes = 2;	break;
		case 0x0f: printf("RRC"); break;
			
		case 0x10: printf("NOP"); break;
		case 0x11: printf("LXI    D,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x12: printf("STAX   D"); break;
		case 0x13: printf("INX    D"); break;
		case 0x14: printf("INR    D"); break;
		case 0x15: printf("DCR    D"); break;
		case 0x16: printf("MVI    D,#$%02x", code[1]); opbytes=2; break;
		case 0x17: printf("RAL"); break;
		case 0x18: printf("NOP"); break;
		case 0x19: printf("DAD    D"); break;
		case 0x1a: printf("LDAX   D"); break;
		case 0x1b: printf("DCX    D"); break;
		case 0x1c: printf("INR    E"); break;
		case 0x1d: printf("DCR    E"); break;
		case 0x1e: printf("MVI    E,#$%02x", code[1]); opbytes = 2; break;
		case 0x1f: printf("RAR"); break;
			
		case 0x20: printf("NOP"); break;
		case 0x21: printf("LXI    H,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x22: printf("SHLD   $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x23: printf("INX    H"); break;
		case 0x24: printf("INR    H"); break;
		case 0x25: printf("DCR    H"); break;
		case 0x26: printf("MVI    H,#$%02x", code[1]); opbytes=2; break;
		case 0x27: printf("DAA"); break;
		case 0x28: printf("NOP"); break;
		case 0x29: printf("DAD    H"); break;
		case 0x2a: printf("LHLD   $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x2b: printf("DCX    H"); break;
		case 0x2c: printf("INR    L"); break;
		case 0x2d: printf("DCR    L"); break;
		case 0x2e: printf("MVI    L,#$%02x", code[1]); opbytes = 2; break;
		case 0x2f: printf("CMA"); break;
			
		case 0x30: printf("NOP"); break;
		case 0x31: printf("LXI    SP,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x32: printf("STA    $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x33: printf("INX    SP"); break;
		case 0x34: printf("INR    M"); break;
		case 0x35: printf("DCR    M"); break;
		case 0x36: printf("MVI    M,#$%02x", code[1]); opbytes=2; break;
		case 0x37: printf("STC"); break;
		case 0x38: printf("NOP"); break;
		case 0x39: printf("DAD    SP"); break;
		case 0x3a: printf("LDA    $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x3b: printf("DCX    SP"); break;
		case 0x3c: printf("INR    A"); break;
		case 0x3d: printf("DCR    A"); break;
		case 0x3e: printf("MVI    A,#$%02x", code[1]); opbytes = 2; break;
		case 0x3f: printf("CMC"); break;
			
		case 0x40: printf("MOV    B,B"); break;
		case 0x41: printf("MOV    B,C"); break;
		case 0x42: printf("MOV    B,D"); break;
		case 0x43: printf("MOV    B,E"); break;
		case 0x44: printf("MOV    B,H"); break;
		case 0x45: printf("MOV    B,L"); break;
		case 0x46: printf("MOV    B,M"); break;
		case 0x47: printf("MOV    B,A"); break;
		case 0x48: printf("MOV    C,B"); break;
		case 0x49: printf("MOV    C,C"); break;
		case 0x4a: printf("MOV    C,D"); break;
		case 0x4b: printf("MOV    C,E"); break;
		case 0x4c: printf("MOV    C,H"); break;
		case 0x4d: printf("MOV    C,L"); break;
		case 0x4e: printf("MOV    C,M"); break;
		case 0x4f: printf("MOV    C,A"); break;
			
		case 0x50: printf("MOV    D,B"); break;
		case 0x51: printf("MOV    D,C"); break;
		case 0x52: printf("MOV    D,D"); break;
		case 0x53: printf("MOV    D.E"); break;
		case 0x54: printf("MOV    D,H"); break;
		case 0x55: printf("MOV    D,L"); break;
		case 0x56: printf("MOV    D,M"); break;
		case 0x57: printf("MOV    D,A"); break;
		case 0x58: printf("MOV    E,B"); break;
		case 0x59: printf("MOV    E,C"); break;
		case 0x5a: printf("MOV    E,D"); break;
		case 0x5b: printf("MOV    E,E"); break;
		case 0x5c: printf("MOV    E,H"); break;
		case 0x5d: printf("MOV    E,L"); break;
		case 0x5e: printf("MOV    E,M"); break;
		case 0x5f: printf("MOV    E,A"); break;

		case 0x60: printf("MOV    H,B"); break;
		case 0x61: printf("MOV    H,C"); break;
		case 0x62: printf("MOV    H,D"); break;
		case 0x63: printf("MOV    H.E"); break;
		case 0x64: printf("MOV    H,H"); break;
		case 0x65: printf("MOV    H,L"); break;
		case 0x66: printf("MOV    H,M"); break;
		case 0x67: printf("MOV    H,A"); break;
		case 0x68: printf("MOV    L,B"); break;
		case 0x69: printf("MOV    L,C"); break;
		case 0x6a: printf("MOV    L,D"); break;
		case 0x6b: printf("MOV    L,E"); break;
		case 0x6c: printf("MOV    L,H"); break;
		case 0x6d: printf("MOV    L,L"); break;
		case 0x6e: printf("MOV    L,M"); break;
		case 0x6f: printf("MOV    L,A"); break;

		case 0x70: printf("MOV    M,B"); break;
		case 0x71: printf("MOV    M,C"); break;
		case 0x72: printf("MOV    M,D"); break;
		case 0x73: printf("MOV    M.E"); break;
		case 0x74: printf("MOV    M,H"); break;
		case 0x75: printf("MOV    M,L"); break;
		case 0x76: printf("HLT");        break;
		case 0x77: printf("MOV    M,A"); break;
		case 0x78: printf("MOV    A,B"); break;
		case 0x79: printf("MOV    A,C"); break;
		case 0x7a: printf("MOV    A,D"); break;
		case 0x7b: printf("MOV    A,E"); break;
		case 0x7c: printf("MOV    A,H"); break;
		case 0x7d: printf("MOV    A,L"); break;
		case 0x7e: printf("MOV    A,M"); break;
		case 0x7f: printf("MOV    A,A"); break;

		case 0x80: printf("ADD    B"); break;
		case 0x81: printf("ADD    C"); break;
		case 0x82: printf("ADD    D"); break;
		case 0x83: printf("ADD    E"); break;
		case 0x84: printf("ADD    H"); break;
		case 0x85: printf("ADD    L"); break;
		case 0x86: printf("ADD    M"); break;
		case 0x87: printf("ADD    A"); break;
		case 0x88: printf("ADC    B"); break;
		case 0x89: printf("ADC    C"); break;
		case 0x8a: printf("ADC    D"); break;
		case 0x8b: printf("ADC    E"); break;
		case 0x8c: printf("ADC    H"); break;
		case 0x8d: printf("ADC    L"); break;
		case 0x8e: printf("ADC    M"); break;
		case 0x8f: printf("ADC    A"); break;

		case 0x90: printf("SUB    B"); break;
		case 0x91: printf("SUB    C"); break;
		case 0x92: printf("SUB    D"); break;
		case 0x93: printf("SUB    E"); break;
		case 0x94: printf("SUB    H"); break;
		case 0x95: printf("SUB    L"); break;
		case 0x96: printf("SUB    M"); break;
		case 0x97: printf("SUB    A"); break;
		case 0x98: printf("SBB    B"); break;
		case 0x99: printf("SBB    C"); break;
		case 0x9a: printf("SBB    D"); break;
		case 0x9b: printf("SBB    E"); break;
		case 0x9c: printf("SBB    H"); break;
		case 0x9d: printf("SBB    L"); break;
		case 0x9e: printf("SBB    M"); break;
		case 0x9f: printf("SBB    A"); break;

		case 0xa0: printf("ANA    B"); break;
		case 0xa1: printf("ANA    C"); break;
		case 0xa2: printf("ANA    D"); break;
		case 0xa3: printf("ANA    E"); break;
		case 0xa4: printf("ANA    H"); break;
		case 0xa5: printf("ANA    L"); break;
		case 0xa6: printf("ANA    M"); break;
		case 0xa7: printf("ANA    A"); break;
		case 0xa8: printf("XRA    B"); break;
		case 0xa9: printf("XRA    C"); break;
		case 0xaa: printf("XRA    D"); break;
		case 0xab: printf("XRA    E"); break;
		case 0xac: printf("XRA    H"); break;
		case 0xad: printf("XRA    L"); break;
		case 0xae: printf("XRA    M"); break;
		case 0xaf: printf("XRA    A"); break;

		case 0xb0: printf("ORA    B"); break;
		case 0xb1: printf("ORA    C"); break;
		case 0xb2: printf("ORA    D"); break;
		case 0xb3: printf("ORA    E"); break;
		case 0xb4: printf("ORA    H"); break;
		case 0xb5: printf("ORA    L"); break;
		case 0xb6: printf("ORA    M"); break;
		case 0xb7: printf("ORA    A"); break;
		case 0xb8: printf("CMP    B"); break;
		case 0xb9: printf("CMP    C"); break;
		case 0xba: printf("CMP    D"); break;
		case 0xbb: printf("CMP    E"); break;
		case 0xbc: printf("CMP    H"); break;
		case 0xbd: printf("CMP    L"); break;
		case 0xbe: printf("CMP    M"); break;
		case 0xbf: printf("CMP    A"); break;

		case 0xc0: printf("RNZ"); break;
		case 0xc1: printf("POP    B"); break;
		case 0xc2: printf("JNZ    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xc3: printf("JMP    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xc4: printf("CNZ    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xc5: printf("PUSH   B"); break;
		case 0xc6: printf("ADI    #$%02x",code[1]); opbytes = 2; break;
		case 0xc7: printf("RST    0"); break;
		case 0xc8: printf("RZ"); break;
		case 0xc9: printf("RET"); break;
		case 0xca: printf("JZ     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xcb: printf("JMP    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xcc: printf("CZ     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xcd: printf("CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xce: printf("ACI    #$%02x",code[1]); opbytes = 2; break;
		case 0xcf: printf("RST    1"); break;

		case 0xd0: printf("RNC"); break;
		case 0xd1: printf("POP    D"); break;
		case 0xd2: printf("JNC    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xd3: printf("OUT    #$%02x",code[1]); opbytes = 2; break;
		case 0xd4: printf("CNC    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xd5: printf("PUSH   D"); break;
		case 0xd6: printf("SUI    #$%02x",code[1]); opbytes = 2; break;
		case 0xd7: printf("RST    2"); break;
		case 0xd8: printf("RC");  break;
		case 0xd9: printf("RET"); break;
		case 0xda: printf("JC     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xdb: printf("IN     #$%02x",code[1]); opbytes = 2; break;
		case 0xdc: printf("CC     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xdd: printf("CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xde: printf("SBI    #$%02x",code[1]); opbytes = 2; break;
		case 0xdf: printf("RST    3"); break;

		case 0xe0: printf("RPO"); break;
		case 0xe1: printf("POP    H"); break;
		case 0xe2: printf("JPO    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xe3: printf("XTHL");break;
		case 0xe4: printf("CPO    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xe5: printf("PUSH   H"); break;
		case 0xe6: printf("ANI    #$%02x",code[1]); opbytes = 2; break;
		case 0xe7: printf("RST    4"); break;
		case 0xe8: printf("RPE"); break;
		case 0xe9: printf("PCHL");break;
		case 0xea: printf("JPE    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xeb: printf("XCHG"); break;
		case 0xec: printf("CPE     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xed: printf("CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xee: printf("XRI    #$%02x",code[1]); opbytes = 2; break;
		case 0xef: printf("RST    5"); break;

		case 0xf0: printf("RP");  break;
		case 0xf1: printf("POP    PSW"); break;
		case 0xf2: printf("JP     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xf3: printf("DI");  break;
		case 0xf4: printf("CP     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xf5: printf("PUSH   PSW"); break;
		case 0xf6: printf("ORI    #$%02x",code[1]); opbytes = 2; break;
		case 0xf7: printf("RST    6"); break;
		case 0xf8: printf("RM");  break;
		case 0xf9: printf("SPHL");break;
		case 0xfa: printf("JM     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xfb: printf("EI");  break;
		case 0xfc: printf("CM     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xfd: printf("CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xfe: printf("CPI    #$%02x",code[1]); opbytes = 2; break;
		case 0xff: printf("RST    7"); break;
	}
	
	return opbytes;
}

typedef struct condition_codes {    
    uint8_t    z:1;    
    uint8_t    s:1;    
    uint8_t    p:1;    
    uint8_t    cy:1;    
    uint8_t    ac:1;    
    uint8_t    pad:3;
} condition_codes_tt;

typedef struct cpu_state {
    // our registers
    uint8_t a, b, c, d, e, h, l;

    uint16_t sp;            // stack pointer
    uint16_t pc;            // program counter
    bool     int_enable;    // irq handling flag

    condition_codes_tt cc;
    uint8_t           *memory;
} State8080;


void unimplementedInstruction(State8080* state)    
{    
  // pc will have advanced one, so undo that    
  printf ("Error: Unimplemented instruction\n");    
	state->pc--;
	Disassemble8080Op(state->memory, state->pc);
	printf("\n");
  delete state;
  exit(1);    
}


int parity(int x, int size)
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

void LogicFlagsA(State8080 *state)
{
	state->cc.cy = state->cc.ac = 0;
	state->cc.z = (state->a == 0);
	state->cc.s = (0x80 == (state->a & 0x80));
	state->cc.p = parity(state->a, 8);
}

void ArithFlagsA(State8080 *state, uint16_t res)
{
	state->cc.cy = (res > 0xff);
	state->cc.z = ((res&0xff) == 0);
	state->cc.s = (0x80 == (res & 0x80));
	state->cc.p = parity(res&0xff, 8);
}

int16_t emulate8080Op(State8080* state)    
{    
	int cycles = 4;
  unsigned char *opcode = &state->memory[state->pc];    
  state->pc+=1;  //for the opcode    

	Disassemble8080Op(state->memory, state->pc);

  switch(*opcode)    
  {    
      case 0x00: break;    
      case 0x01: 
            state->c = opcode[1];    
            state->b = opcode[2];    
            state->pc += 2;                  //Advance 2 more bytes    
            break;    

      case 0x02: unimplementedInstruction(state); break;    
      case 0x03: unimplementedInstruction(state); break;    
      case 0x04: unimplementedInstruction(state); break;    
      case 0xfe: unimplementedInstruction(state); break;    
      case 0xff: unimplementedInstruction(state); break;    
      default: unimplementedInstruction(state); break;    
  }    

  printf("\t");
	printf("%c", state->cc.z ? 'z' : '.');
	printf("%c", state->cc.s ? 's' : '.');
	printf("%c", state->cc.p ? 'p' : '.');
	printf("%c", state->cc.cy ? 'c' : '.');
	printf("%c  ", state->cc.ac ? 'a' : '.');
	printf("A $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n", state->a, state->b, state->c,
				state->d, state->e, state->h, state->l, state->sp);
	return 0;

} 

int16_t dissassemble8080Instruction(bin_array_const_it & it, int pc) {
  byte opcode = *it;
  int opbytes = 1;

  cout << hex << setfill('0');
  cout << setw(4) << pc << " " << setw(2) << std::to_integer<int>(opcode) << " ";
  // cout << std::left << setfill(' ') << setw();
  switch(std::to_integer<unsigned char>(opcode)) {
    case 0x00 : cout << "NOP        " ; opbytes = 1; break;
    case 0x01 : cout << "LXI    B,#$" ; opbytes = 3; break;
    case 0x02 : cout << "STAX   B   " ; opbytes = 1; break;
    case 0x03 : cout << "INX    B   " ; opbytes = 1; break;
    case 0x04 : cout << "INR    B   " ; opbytes = 1; break;
    case 0x05 : cout << "DCR    B   " ; opbytes = 1; break;
    case 0x06 : cout << "MVI    B,#$" ; opbytes = 2; break;
    case 0x07 : cout << "RLC        " ; opbytes = 1; break;
    case 0x08 : cout << "NOP        " ; opbytes = 1; break;
    case 0x09 : cout << "DAD    B   " ; opbytes = 1; break;
    case 0x0A : cout << "LDAX   B   " ; opbytes = 1; break;
    case 0x0B : cout << "DCX    B   " ; opbytes = 1; break;
    case 0x0C : cout << "INR    C   " ; opbytes = 1; break;
    case 0x0D : cout << "DCR    C   " ; opbytes = 1; break;
    case 0x0E : cout << "MVI    C,#$" ; opbytes = 2; break;
    case 0x0F : cout << "RRC        " ; opbytes = 1; break;

    case 0x10 : cout << "NOP        " ; opbytes = 1; break;
    case 0x11 : cout << "LXI    D,#$" ; opbytes = 3; break;
    case 0x12 : cout << "STAX   D   " ; opbytes = 1; break;
    case 0x13 : cout << "INX    D   " ; opbytes = 1; break;
    case 0x14 : cout << "INR    D   " ; opbytes = 1; break;
    case 0x15 : cout << "DCR    D   " ; opbytes = 1; break;
    case 0x16 : cout << "MVI    D,#$" ; opbytes = 2; break;
    case 0x17 : cout << "RAL        " ; opbytes = 1; break;
    case 0x18 : cout << "NOP        " ; opbytes = 1; break;
    case 0x19 : cout << "DAD    D   " ; opbytes = 1; break;
    case 0x1A : cout << "LDAX   D   " ; opbytes = 1; break;
    case 0x1B : cout << "DCX    D   " ; opbytes = 1; break;
    case 0x1C : cout << "INR    E   " ; opbytes = 1; break;
    case 0x1D : cout << "DCR    E   " ; opbytes = 1; break;
    case 0x1E : cout << "MVI    E,#$" ; opbytes = 2; break;
    case 0x1F : cout << "RAC        " ; opbytes = 1; break;

    case 0x20 : cout << "NOP        " ; opbytes = 1; break;
    case 0x21 : cout << "LXI    H,#$" ; opbytes = 3; break;
    case 0x22 : cout << "SHLD   $"    ; opbytes = 3; break;
    case 0x23 : cout << "INX    H   " ; opbytes = 1; break;
    case 0x24 : cout << "INR    H   " ; opbytes = 1; break;
    case 0x25 : cout << "DCR    H   " ; opbytes = 1; break;
    case 0x26 : cout << "MVI    H,#$" ; opbytes = 2; break;
    case 0x27 : cout << "DAA        " ; opbytes = 1; break;
    case 0x28 : cout << "NOP        " ; opbytes = 1; break;
    case 0x29 : cout << "DAD    H   " ; opbytes = 1; break;
    case 0x2A : cout << "LHLD   $" ;    opbytes = 3; break;
    case 0x2B : cout << "DCX    H   " ; opbytes = 1; break;
    case 0x2C : cout << "INR    L   " ; opbytes = 1; break;
    case 0x2D : cout << "DCR    L   " ; opbytes = 1; break;
    case 0x2E : cout << "MVI    L,#$" ; opbytes = 2; break;
    case 0x2F : cout << "CMA        " ; opbytes = 1; break;

    case 0x30 : cout << "NOP        " ; opbytes = 1; break;
    case 0x31 : cout << "LXI    SP,#$"; opbytes = 3; break;
    case 0x32 : cout << "STA    $"    ; opbytes = 3; break;
    case 0x33 : cout << "INX    SP  " ; opbytes = 1; break;
    case 0x34 : cout << "INR    M   " ; opbytes = 1; break;
    case 0x35 : cout << "DCR    M   " ; opbytes = 1; break;
    case 0x36 : cout << "MVI    M,#$" ; opbytes = 2; break;
    case 0x37 : cout << "STC        " ; opbytes = 1; break;
    case 0x38 : cout << "NOP        " ; opbytes = 1; break;
    case 0x39 : cout << "DAD    SP  " ; opbytes = 1; break;
    case 0x3A : cout << "LDA    $"    ; opbytes = 3; break;
    case 0x3B : cout << "DCX    SP  " ; opbytes = 1; break;
    case 0x3C : cout << "INR    A   " ; opbytes = 1; break;
    case 0x3D : cout << "DCR    A   " ; opbytes = 1; break;
    case 0x3E : cout << "MVI    A,#$" ; opbytes = 2; break;
    case 0x3F : cout << "CMC        " ; opbytes = 1; break;

    case 0x40 : cout << "MOV    B,B " ; opbytes = 1; break;
    case 0x41 : cout << "MOV    B,C " ; opbytes = 1; break;
    case 0x42 : cout << "MOV    B,D " ; opbytes = 1; break;
    case 0x43 : cout << "MOV    B,E " ; opbytes = 1; break;
    case 0x44 : cout << "MOV    B,H " ; opbytes = 1; break;
    case 0x45 : cout << "MOV    B,L " ; opbytes = 1; break;
    case 0x46 : cout << "MOV    B,M " ; opbytes = 1; break;
    case 0x47 : cout << "MOV    B,A " ; opbytes = 1; break;
    case 0x48 : cout << "MOV    C,B " ; opbytes = 1; break;
    case 0x49 : cout << "MOV    C,C " ; opbytes = 1; break;
    case 0x4A : cout << "MOV    C,D " ; opbytes = 1; break;
    case 0x4B : cout << "MOV    C,E " ; opbytes = 1; break;
    case 0x4C : cout << "MOV    C,H " ; opbytes = 1; break;
    case 0x4D : cout << "MOV    C,L " ; opbytes = 1; break;
    case 0x4E : cout << "MOV    C,M " ; opbytes = 1; break;
    case 0x4F : cout << "MOV    C,A " ; opbytes = 1; break;

    case 0x50 : cout << "MOV    D,B " ; opbytes = 1; break;
    case 0x51 : cout << "MOV    D,C " ; opbytes = 1; break;
    case 0x52 : cout << "MOV    D,D " ; opbytes = 1; break;
    case 0x53 : cout << "MOV    D,E " ; opbytes = 1; break;
    case 0x54 : cout << "MOV    D,H " ; opbytes = 1; break;
    case 0x55 : cout << "MOV    D,L " ; opbytes = 1; break;
    case 0x56 : cout << "MOV    D,M " ; opbytes = 1; break;
    case 0x57 : cout << "MOV    D,A " ; opbytes = 1; break;
    case 0x58 : cout << "MOV    E,B " ; opbytes = 1; break;
    case 0x59 : cout << "MOV    E,C " ; opbytes = 1; break;
    case 0x5A : cout << "MOV    E,D " ; opbytes = 1; break;
    case 0x5B : cout << "MOV    E,E " ; opbytes = 1; break;
    case 0x5C : cout << "MOV    E,H " ; opbytes = 1; break;
    case 0x5D : cout << "MOV    E,L " ; opbytes = 1; break;
    case 0x5E : cout << "MOV    E,M " ; opbytes = 1; break;
    case 0x5F : cout << "MOV    E,A " ; opbytes = 1; break;

    case 0x60 : cout << "MOV    H,B " ; opbytes = 1; break;
    case 0x61 : cout << "MOV    H,C " ; opbytes = 1; break;
    case 0x62 : cout << "MOV    H,D " ; opbytes = 1; break;
    case 0x63 : cout << "MOV    H,E " ; opbytes = 1; break;
    case 0x64 : cout << "MOV    H,H " ; opbytes = 1; break;
    case 0x65 : cout << "MOV    H,L " ; opbytes = 1; break;
    case 0x66 : cout << "MOV    H,M " ; opbytes = 1; break;
    case 0x67 : cout << "MOV    H,A " ; opbytes = 1; break;
    case 0x68 : cout << "MOV    L,B " ; opbytes = 1; break;
    case 0x69 : cout << "MOV    L,C " ; opbytes = 1; break;
    case 0x6A : cout << "MOV    L,D " ; opbytes = 1; break;
    case 0x6B : cout << "MOV    L,E " ; opbytes = 1; break;
    case 0x6C : cout << "MOV    L,H " ; opbytes = 1; break;
    case 0x6D : cout << "MOV    L,L " ; opbytes = 1; break;
    case 0x6E : cout << "MOV    L,M " ; opbytes = 1; break;
    case 0x6F : cout << "MOV    L,A " ; opbytes = 1; break;

    case 0x70 : cout << "MOV    M,B " ; opbytes = 1; break;
    case 0x71 : cout << "MOV    M,C " ; opbytes = 1; break;
    case 0x72 : cout << "MOV    M,D " ; opbytes = 1; break;
    case 0x73 : cout << "MOV    M,E " ; opbytes = 1; break;
    case 0x74 : cout << "MOV    M,H " ; opbytes = 1; break;
    case 0x75 : cout << "MOV    M,L " ; opbytes = 1; break;
    case 0x76 : cout << "HLT        " ; opbytes = 1; break;
    case 0x77 : cout << "MOV    M,A " ; opbytes = 1; break;
    case 0x78 : cout << "MOV    A,B " ; opbytes = 1; break;
    case 0x79 : cout << "MOV    A,C " ; opbytes = 1; break;
    case 0x7A : cout << "MOV    A,D " ; opbytes = 1; break;
    case 0x7B : cout << "MOV    A,E " ; opbytes = 1; break;
    case 0x7C : cout << "MOV    A,H " ; opbytes = 1; break;
    case 0x7D : cout << "MOV    A,L " ; opbytes = 1; break;
    case 0x7E : cout << "MOV    A,M " ; opbytes = 1; break;
    case 0x7F : cout << "MOV    A,A " ; opbytes = 1; break;

    case 0x80 : cout << "ADD    B   " ; opbytes = 1; break;
    case 0x81 : cout << "ADD    C   " ; opbytes = 1; break;
    case 0x82 : cout << "ADD    D   " ; opbytes = 1; break;
    case 0x83 : cout << "ADD    E   " ; opbytes = 1; break;
    case 0x84 : cout << "ADD    H   " ; opbytes = 1; break;
    case 0x85 : cout << "ADD    L   " ; opbytes = 1; break;
    case 0x86 : cout << "ADD    M   " ; opbytes = 1; break;
    case 0x87 : cout << "ADD    A   " ; opbytes = 1; break;
    case 0x88 : cout << "ADC    B   " ; opbytes = 1; break;
    case 0x89 : cout << "ADC    C   " ; opbytes = 1; break;
    case 0x8A : cout << "ADC    D   " ; opbytes = 1; break;
    case 0x8B : cout << "ADC    E   " ; opbytes = 1; break;
    case 0x8C : cout << "ADC    H   " ; opbytes = 1; break;
    case 0x8D : cout << "ADC    L   " ; opbytes = 1; break;
    case 0x8E : cout << "ADC    M   " ; opbytes = 1; break;
    case 0x8F : cout << "ADC    A   " ; opbytes = 1; break;

    case 0x90 : cout << "SUB    B   " ; opbytes = 1; break;
    case 0x91 : cout << "SUB    C   " ; opbytes = 1; break;
    case 0x92 : cout << "SUB    D   " ; opbytes = 1; break;
    case 0x93 : cout << "SUB    E   " ; opbytes = 1; break;
    case 0x94 : cout << "SUB    H   " ; opbytes = 1; break;
    case 0x95 : cout << "SUB    L   " ; opbytes = 1; break;
    case 0x96 : cout << "SUB    M   " ; opbytes = 1; break;
    case 0x97 : cout << "SUB    A   " ; opbytes = 1; break;
    case 0x98 : cout << "SBB    B   " ; opbytes = 1; break;
    case 0x99 : cout << "SBB    C   " ; opbytes = 1; break;
    case 0x9A : cout << "SBB    D   " ; opbytes = 1; break;
    case 0x9B : cout << "SBB    E   " ; opbytes = 1; break;
    case 0x9C : cout << "SBB    H   " ; opbytes = 1; break;
    case 0x9D : cout << "SBB    L   " ; opbytes = 1; break;
    case 0x9E : cout << "SBB    M   " ; opbytes = 1; break;
    case 0x9F : cout << "SBB    A   " ; opbytes = 1; break;

    case 0xA0 : cout << "ANA    B   " ; opbytes = 1; break;
    case 0xA1 : cout << "ANA    C   " ; opbytes = 1; break;
    case 0xA2 : cout << "ANA    D   " ; opbytes = 1; break;
    case 0xA3 : cout << "ANA    E   " ; opbytes = 1; break;
    case 0xA4 : cout << "ANA    H   " ; opbytes = 1; break;
    case 0xA5 : cout << "ANA    L   " ; opbytes = 1; break;
    case 0xA6 : cout << "ANA    M   " ; opbytes = 1; break;
    case 0xA7 : cout << "ANA    A   " ; opbytes = 1; break;
    case 0xA8 : cout << "XRA    B   " ; opbytes = 1; break;
    case 0xA9 : cout << "XRA    C   " ; opbytes = 1; break;
    case 0xAA : cout << "XRA    D   " ; opbytes = 1; break;
    case 0xAB : cout << "XRA    E   " ; opbytes = 1; break;
    case 0xAC : cout << "XRA    H   " ; opbytes = 1; break;
    case 0xAD : cout << "XRA    L   " ; opbytes = 1; break;
    case 0xAE : cout << "XRA    M   " ; opbytes = 1; break;
    case 0xAF : cout << "XRA    A   " ; opbytes = 1; break;

    case 0xB0 : cout << "ORA    B   " ; opbytes = 1; break;
    case 0xB1 : cout << "ORA    C   " ; opbytes = 1; break;
    case 0xB2 : cout << "ORA    D   " ; opbytes = 1; break;
    case 0xB3 : cout << "ORA    E   " ; opbytes = 1; break;
    case 0xB4 : cout << "ORA    H   " ; opbytes = 1; break;
    case 0xB5 : cout << "ORA    L   " ; opbytes = 1; break;
    case 0xB6 : cout << "ORA    M   " ; opbytes = 1; break;
    case 0xB7 : cout << "ORA    A   " ; opbytes = 1; break;
    case 0xB8 : cout << "CMP    B   " ; opbytes = 1; break;
    case 0xB9 : cout << "CMP    C   " ; opbytes = 1; break;
    case 0xBA : cout << "CMP    D   " ; opbytes = 1; break;
    case 0xBB : cout << "CMP    E   " ; opbytes = 1; break;
    case 0xBC : cout << "CMP    H   " ; opbytes = 1; break;
    case 0xBD : cout << "CMP    L   " ; opbytes = 1; break;
    case 0xBE : cout << "CMP    M   " ; opbytes = 1; break;
    case 0xBF : cout << "CMP    A   " ; opbytes = 1; break;

    case 0xC0 : cout << "RNZ        " ; opbytes = 1; break;
    case 0xC1 : cout << "POP    B   " ; opbytes = 1; break;
    case 0xC2 : cout << "JNZ    $"    ; opbytes = 3; break;
    case 0xC3 : cout << "JMP    $"    ; opbytes = 3; break;
    case 0xC4 : cout << "CNZ    $"    ; opbytes = 3; break;
    case 0xC5 : cout << "PUSH   B   " ; opbytes = 1; break;
    case 0xC6 : cout << "ADI    #$"   ; opbytes = 2; break;
    case 0xC7 : cout << "RST    0   " ; opbytes = 1; break;
    case 0xC8 : cout << "RZ         " ; opbytes = 1; break;
    case 0xC9 : cout << "RET        " ; opbytes = 1; break;
    case 0xCA : cout << "JZ     $"    ; opbytes = 3; break;
    case 0xCB : cout << "JMP    $"    ; opbytes = 3; break;
    case 0xCC : cout << "CZ     $"    ; opbytes = 3; break;
    case 0xCD : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xCE : cout << "ACI    $"    ; opbytes = 2; break;
    case 0xCF : cout << "RST    1   " ; opbytes = 1; break;

    case 0xD0 : cout << "RNC        " ; opbytes = 1; break;
    case 0xD1 : cout << "POP    D   " ; opbytes = 1; break;
    case 0xD2 : cout << "JNC    $"    ; opbytes = 3; break;
    case 0xD3 : cout << "OUT    $"    ; opbytes = 2; break;
    case 0xD4 : cout << "CNC    $"    ; opbytes = 3; break;
    case 0xD5 : cout << "PUSH   D   " ; opbytes = 1; break;
    case 0xD6 : cout << "SUI    #$"   ; opbytes = 2; break;
    case 0xD7 : cout << "RST    2   " ; opbytes = 1; break;
    case 0xD8 : cout << "RC         " ; opbytes = 1; break;
    case 0xD9 : cout << "RET        " ; opbytes = 1; break;
    case 0xDA : cout << "JC     $"    ; opbytes = 3; break;
    case 0xDB : cout << "IN     $"    ; opbytes = 2; break;
    case 0xDC : cout << "CC     $"    ; opbytes = 3; break;
    case 0xDD : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xDE : cout << "SBI    $"    ; opbytes = 2; break;
    case 0xDF : cout << "RST    3   " ; opbytes = 1; break;

    case 0xE0 : cout << "RPO        " ; opbytes = 1; break;
    case 0xE1 : cout << "POP    H   " ; opbytes = 1; break;
    case 0xE2 : cout << "JPO    $"    ; opbytes = 3; break;
    case 0xE3 : cout << "XTHL       " ; opbytes = 1; break;
    case 0xE4 : cout << "CPO    $"    ; opbytes = 3; break;
    case 0xE5 : cout << "PUSH   H   " ; opbytes = 1; break;
    case 0xE6 : cout << "ANI    #$"   ; opbytes = 2; break;
    case 0xE7 : cout << "RST    4   " ; opbytes = 1; break;
    case 0xE8 : cout << "RPE        " ; opbytes = 1; break;
    case 0xE9 : cout << "PCHL       " ; opbytes = 1; break;
    case 0xEA : cout << "JPE    $"    ; opbytes = 3; break;
    case 0xEB : cout << "XCHG       " ; opbytes = 1; break;
    case 0xEC : cout << "CPE    $"    ; opbytes = 3; break;
    case 0xED : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xEE : cout << "XRI    $"    ; opbytes = 2; break;
    case 0xEF : cout << "RST    5   " ; opbytes = 1; break;

    case 0xF0 : cout << "RP         " ; opbytes = 1; break;
    case 0xF1 : cout << "POP    PSW " ; opbytes = 1; break;
    case 0xF2 : cout << "JP     $"    ; opbytes = 3; break;
    case 0xF3 : cout << "DI         " ; opbytes = 1; break;
    case 0xF4 : cout << "CP     $"    ; opbytes = 3; break;
    case 0xF5 : cout << "PUSH   PSW " ; opbytes = 1; break;
    case 0xF6 : cout << "ORI    #$"   ; opbytes = 2; break;
    case 0xF7 : cout << "RST    6   " ; opbytes = 1; break;
    case 0xF8 : cout << "RM         " ; opbytes = 1; break;
    case 0xF9 : cout << "SPHL       " ; opbytes = 1; break;
    case 0xFA : cout << "JM     $"    ; opbytes = 3; break;
    case 0xFB : cout << "EI         " ; opbytes = 1; break;
    case 0xFC : cout << "CM     $"    ; opbytes = 3; break;
    case 0xFD : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xFE : cout << "CPI    $"    ; opbytes = 2; break;
    case 0xFF : cout << "RST    7   " ; opbytes = 1; break;

    default   : cout << "UNK"; break;
  }

  if (opbytes == 3) {
    auto b2 = std::to_integer<int>(*(it+2));
    auto b1 = std::to_integer<int>(*(it+1));
    cout << hex << setfill('0');
    cout  << setw(2) << b2  << setw(2) << b1;
  } else if (opbytes == 2) {
    auto b1 = std::to_integer<int>(*(it+1));
    cout << hex << setfill('0') << setw(2);
    cout << b1;
  }

  // it+=opbytes;
  cout << endl;
  return opbytes;
}

void dissassemble8080Blob(bin_array const& binaryCode) {
  // cout << hex << setfill('0');  // needs to be set only once
  auto it = binaryCode.begin();
  while (it != binaryCode.end()) {
    auto opbytes = dissassemble8080Instruction(it, it - binaryCode.begin());
    it+=opbytes; 
  }
}

bin_array load_file(std::string const& filepath) {
  std::ifstream ifs(filepath, std::ios::binary|std::ios::ate);
  if (!ifs) throw std::runtime_error(filepath + ": " + std::strerror(errno));

  auto  end = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  auto size = std::size_t(end - ifs.tellg());
  if (size == 0) return {};

  bin_array buffer(size);
  if (!ifs.read((char*) buffer.data(), buffer.size())) {
    throw std::runtime_error(filepath + ": " + std::strerror(errno));
  }

  return buffer;
}

void ReadFileIntoMemoryAt(State8080* state, char* filename, uint32_t offset)
{
	FILE *f= fopen(filename, "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

void ReadFileIntoBufferAt(uint8_t* memory, char* filename, uint32_t offset)
{
	FILE *f= fopen(filename, "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	uint8_t *buffer = &memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

State8080* Init8080(void)
{
	State8080* state = new State8080();
	state->memory = (uint8_t*) malloc(0x10000);  //16K
	return state;
}

int main (int argc, char**argv) {
  // auto file = string(argv[1]);
  // auto fileContent = load_file(file);
  // std::cout << "File content size is: " << fileContent.size() << std::endl;
  // dissassemble8080Blob(fileContent);

  auto fileName = argv[1];
	int done = 0;

  auto core = cpu();
  core.load_instruction_set();
  ReadFileIntoBufferAt(core.memory, fileName, 0);
	while (done == 0)
	{
    core.step();
  }

	auto state = Init8080();

	ReadFileIntoMemoryAt(state, fileName, 0);

	while (done == 0)
	{
		done = emulate8080Op(state);
	}

  delete state;

  return 0;
}
