
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

_cpu_state::_cpu_state() {
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
      cout << (a == 0 ? "YES" : "NO") << endl;

}

_cpu_state::~_cpu_state() {
    cout << "here" << endl;
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

