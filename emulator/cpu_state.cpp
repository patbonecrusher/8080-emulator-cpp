
#include <cstddef>
#include <iostream>
#include <iomanip>

#include "cpu_state.hpp"

using namespace std;

_condition_codes::_condition_codes() {
    z = 0;
    s = 0;
    p = 0;
    cy = 0;
    ac = 0;
    pad = 0;
}

ostream& operator<<(ostream& os, const _condition_codes& cc) {
    os << (cc.z ? 'z' : '.')
       << (cc.s ? 's' : '.')
       << (cc.p ? 'p' : '.')
       << (cc.cy ? 'c' : '.')
       << (cc.ac ? 'a' : '.');
    return os;
}

_cpu_state::_cpu_state(uint8_t  *memory) 
    : memory(memory){
    a = 0;
    b = 0;
    c = 0;
    d = 0;
    e = 0;
    h = 0;
    l = 0;
    sp = 0;
    pc = 0;
    int_enable = false;
}

_cpu_state::~_cpu_state() {
    cout << "here" << endl;
}

static int parity(int x, int size) {
	int i;
	int p = 0;
	x = (x & ((1<<size)-1));
	for (i=0; i<size; i++) {
		if (x & 0x1) p++;
		x = x >> 1;
	}
	return (0 == (p & 0x1));
}

void _cpu_state::logic_flags_a(uint16_t value) {
    a = value;
	cc.cy = cc.ac = 0;
	cc.z = (a == 0);
	cc.s = (0x80 == (a & 0x80));
	cc.p = parity(a, 8);
}

void _cpu_state::write_mem(uint16_t address, uint8_t value) {
    if (address < 0x2000)
    {
        printf("Writing ROM not allowed %x\n", address);
        return;
    }
    if (address >=0x4000)
    {
        printf("Writing out of Space Invaders RAM not allowed %x\n", address);
        return;
    }
    
    memory[address] = value;
}


ostream& operator<<(ostream& os, const _cpu_state& cs) {
    os  << cs.cc;
    os  << " A $" << hex << setw(2) << setfill('0') << (int) cs.a
        << " B $" << hex << setw(2) << setfill('0') << (int) cs.b
        << " C $" << hex << setw(2) << setfill('0') << (int) cs.c
        << " D $" << hex << setw(2) << setfill('0') << (int) cs.d
        << " E $" << hex << setw(2) << setfill('0') << (int) cs.e
        << " L $" << hex << setw(2) << setfill('0') << (int) cs.l
        << " SP " << hex << setfill('0') << setw(4) << (int) cs.sp;
    return os;
}

