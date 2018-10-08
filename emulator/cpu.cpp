#include <iomanip>

#include "instruction_set.hpp"
#include "cpu.hpp"

cpu::cpu()
    : memory(new uint8_t[0x10000])
    , cpu_state(memory)
{
}

cpu::~cpu() {
    delete[] memory;
}

void cpu::load_firmware_in_memory(uint8_t *mem_buffer, uint16_t address) {
    
}

void cpu::step() {
    uint8_t opcode = memory[cpu_state.pc];    

    this->instructions[opcode].dissassemble(&memory[cpu_state.pc+1], cpu_state.pc);
    this->instructions[opcode].execute(&memory[cpu_state.pc+1], cpu_state);
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

// void jmp(uint8_t *data, cpu_state_t& state) {
//     state.pc = data[1] << 8 | data[0];
// }

// void fe_cpi(uint8_t *data, cpu_state_t& state) {
//     uint8_t x = state.a - data[0];
//     FlagsZSP(state, x);
//     state.cc.cy = (state.a < data[0]);
//     state.pc++;
// }

void unimpl(uint8_t * opcode, cpu_state_t& state) {
    cout << "error: unimplement instruction" << endl;
    exit(1);
}

#define ADD_INS(addr, args ...) this->instructions[addr] = instruction(addr, args)

void cpu::load_instruction_set() {
    for (int i=0; i<255; ++i) {
        this->instructions[i] = instruction(i, "ERROR", 0, 1, unimpl);
    }
    
    // +++++++ START: Misc/Control instructions
    ADD_INS(0x00, "NOP", 4);
    ADD_INS(0x10, "NOP", 4);
    ADD_INS(0x20, "NOP", 4);
    ADD_INS(0x30, "NOP", 4);
    ADD_INS(0x08, "NOP", 4);
    ADD_INS(0x18, "NOP", 4);
    ADD_INS(0x28, "NOP", 4);
    ADD_INS(0x38, "NOP", 4);

    ADD_INS(0x76, "HLT", 7);
    ADD_INS(0xd3, "OUT", 10, 2, nullptr);
    ADD_INS(0xdb, "IN" , 10, 2, nullptr);
    ADD_INS(0xf3, "DI" , 4 , 1, _f3_di );
    ADD_INS(0xfb, "EI" , 4 , 1, _fb_ei );
    // +++++++ END: Misc/Control instructions

    // +++++++ Jump/Call instructions
    ADD_INS(0xc0, "RNZ"     , 11, 1, _c0_rnz);
    ADD_INS(0xc2, "JNZ"     , 10, 3, _c2_jnz_a16);
    ADD_INS(0xc3, "JMP"     , 11, 3, _c3_jmp_a16);
    ADD_INS(0xc4, "CNZ"     , 17, 3, _c4_cnz_a16);
    ADD_INS(0xc7, "RST_0"   , 11, 1, _c7_rst_0);
    ADD_INS(0xc8, "RZ"      , 11, 1, _c8_rz);
    ADD_INS(0xc9, "RET"     , 10, 1, _c9_ret);
    ADD_INS(0xca, "JZ"      , 10, 3, _ca_jz_a16);
    ADD_INS(0xcb, "*JMP"    , 10, 3, _cb_jmp_a16_alt);
    ADD_INS(0xcc, "CZ"      , 17, 3, _cc_cz_a16);
    ADD_INS(0xcd, "CALL"    , 17, 3, _cd_call_a16);
    ADD_INS(0xcf, "RST_1"   , 11, 1, _cf_rst_1);

    ADD_INS(0xd0, "RNC"     , 11, 1, _d0_rnc);
    ADD_INS(0xd2, "JNC"     , 10, 3, _d2_jnc_a16);
    ADD_INS(0xd4, "CNC"     , 17, 3, _d4_cnc_a16);
    ADD_INS(0xd7, "RST_2"   , 11, 1, _d7_rst_2);
    ADD_INS(0xd8, "RC"      , 11, 1, _d8_rc);
    ADD_INS(0xd9, "*RET"    , 10, 1, _d9_ret_alt);
    ADD_INS(0xda, "JC"      , 10, 3, _da_jc_a16);
    ADD_INS(0xdc, "CC"      , 17, 3, _dc_cc_a16);
    ADD_INS(0xdd, "*CALL"   , 17, 3, _dd_call_a16_alt);
    ADD_INS(0xdf, "RST_3"   , 11, 1, _df_rst_3);

    ADD_INS(0xe0, "RPO"     , 11, 1, _e0_rpo);
    ADD_INS(0xe2, "JPO"     , 10, 3, _e2_jpo_a16);
    ADD_INS(0xe4, "CPO"     , 17, 3, _e4_cpo_a16);
    ADD_INS(0xe7, "RST_4"   , 11, 1, _e7_rst_4);
    ADD_INS(0xe8, "RPE"     , 11, 1, _e8_rpe);
    ADD_INS(0xe9, "PHCL"    , 10, 1, _e9_phcl);
    ADD_INS(0xea, "JPE"     , 10, 3, _ea_jpe_a16);
    ADD_INS(0xec, "CPE"     , 17, 3, _ec_cpe_a16);
    ADD_INS(0xed, "*CALL"   , 17, 3, _ed_call_a16_alt);
    ADD_INS(0xef, "RST_5"   , 11, 1, _ef_rst_5);

    ADD_INS(0xf0, "RP"      , 11, 1, _f0_rp);
    ADD_INS(0xf2, "JP"      , 10, 3, _f2_jp_a16);
    ADD_INS(0xf4, "CP"      , 17, 3, _f4_cp_a16);
    ADD_INS(0xf7, "RST_6"   , 11, 1, _f7_rst_6);
    ADD_INS(0xf8, "RM"      , 11, 1, _f8_rm);
    ADD_INS(0xfa, "JM"      , 10, 3, _fa_jm_a16);
    ADD_INS(0xfc, "CM"      , 17, 3, _fc_cm_a16);
    ADD_INS(0xfd, "*CALL"   , 17, 3, _fd_call_a16_alt);
    ADD_INS(0xff, "RST_7"   , 11, 1, _ff_rst_7);

    // +++++++ Jump/Calls instructions

    // +++++++ START: 8 bits load/store instructions
    ADD_INS(0x06, "MVI_B_D8", 7, 2, _06_mvi_b_d8);
    // +++++++ END: 16 bits load/store instructions

    // +++++++ START: 16 bits load/store instructions
    ADD_INS(0x11, "LXI_D_D16", 10, 3, _11_lxi_d_d16);
    ADD_INS(0x21, "LXI_H_D16", 10, 3, _21_lxi_h_d16);
    ADD_INS(0x31, "LXI_SP_D16", 10, 3, _31_lxi_sp_d16);
    // +++++++ END: 16 bits load/store instructions

    // this->instructions[0x01] = instruction(0x01, nop0, "LXI    B,#$", 2);
    // this->instructions[0xc3] = instruction(0xc3, jmp , "JMP       $", 2);
    // this->instructions[0xfe] = instruction(0xfe, fe_cpi, "CPI      #$", 2);
}