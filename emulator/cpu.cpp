#include <iomanip>
#include <system_error>

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

int cpu::run() {
    while(1) {
        this->next();
    }
    return 0;
}

void cpu::next() {
    uint8_t opcode = memory[cpu_state.pc];    

    this->instructions[opcode].dissassemble(&memory[cpu_state.pc+1], cpu_state.pc);
    this->instructions[opcode].execute(&memory[cpu_state.pc+1], cpu_state);
    cout << " " << cpu_state << endl;
}

void cpu::halt(uint8_t * opcode, cpu_state_t& state) {
    throw std::system_error(EFAULT, std::generic_category());
}

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

    ADD_INS(0xd3, "OUT", 10, 2, nullptr);
    ADD_INS(0xdb, "IN" , 10, 2, nullptr);
    ADD_INS(0xf3, "DI" , 4 , 1, _f3_di );
    ADD_INS(0xfb, "EI" , 4 , 1, _fb_ei );
    // +++++++ END: Misc/Control instructions

    // +++++++ Jump/Call instructions
    ADD_INS(0xc0, "RNZ"     , 11, 1, _c0_rnz);
    ADD_INS(0xc2, "JNZ"     , 10, 3, _c2_jnz_a16);
    ADD_INS(0xc3, "JMP"     , 10, 3, _c3_jmp_a16);
    ADD_INS(0xc4, "CNZ"     , 17, 3, _c4_cnz_a16);
    ADD_INS(0xc7, "RST_0"   , 11, 1, _c7_rst_0);
    ADD_INS(0xc8, "RZ"      , 11, 1, _c8_rz);
    ADD_INS(0xc9, "RET"     , 10, 1, _c9_ret);
    ADD_INS(0xca, "JZ"      , 10, 3, _ca_jz_a16);
    // ADD_INS(0xcb, "*JMP"    , 10, 3, _cb_jmp_a16_alt);
    ADD_INS(0xcc, "CZ"      , 17, 3, _cc_cz_a16);
    ADD_INS(0xcd, "CALL"    , 17, 3, _cd_call_a16);
    ADD_INS(0xcf, "RST_1"   , 11, 1, _cf_rst_1);

    ADD_INS(0xd0, "RNC"     , 11, 1, _d0_rnc);
    ADD_INS(0xd2, "JNC"     , 10, 3, _d2_jnc_a16);
    ADD_INS(0xd4, "CNC"     , 17, 3, _d4_cnc_a16);
    ADD_INS(0xd7, "RST_2"   , 11, 1, _d7_rst_2);
    ADD_INS(0xd8, "RC"      , 11, 1, _d8_rc);
    // ADD_INS(0xd9, "*RET"    , 10, 1, _d9_ret_alt);
    ADD_INS(0xda, "JC"      , 10, 3, _da_jc_a16);
    ADD_INS(0xdc, "CC"      , 17, 3, _dc_cc_a16);
    // ADD_INS(0xdd, "*CALL"   , 17, 3, _dd_call_a16_alt);
    ADD_INS(0xdf, "RST_3"   , 11, 1, _df_rst_3);

    ADD_INS(0xe0, "RPO"     , 11, 1, _e0_rpo);
    ADD_INS(0xe2, "JPO"     , 10, 3, _e2_jpo_a16);
    ADD_INS(0xe4, "CPO"     , 17, 3, _e4_cpo_a16);
    ADD_INS(0xe7, "RST_4"   , 11, 1, _e7_rst_4);
    ADD_INS(0xe8, "RPE"     , 11, 1, _e8_rpe);
    ADD_INS(0xe9, "PHCL"    , 10, 1, _e9_phcl);
    ADD_INS(0xea, "JPE"     , 10, 3, _ea_jpe_a16);
    ADD_INS(0xec, "CPE"     , 17, 3, _ec_cpe_a16);
    // ADD_INS(0xed, "*CALL"   , 17, 3, _ed_call_a16_alt);
    ADD_INS(0xef, "RST_5"   , 11, 1, _ef_rst_5);

    ADD_INS(0xf0, "RP"      , 11, 1, _f0_rp);
    ADD_INS(0xf2, "JP"      , 10, 3, _f2_jp_a16);
    ADD_INS(0xf4, "CP"      , 17, 3, _f4_cp_a16);
    ADD_INS(0xf7, "RST_6"   , 11, 1, _f7_rst_6);
    ADD_INS(0xf8, "RM"      , 11, 1, _f8_rm);
    ADD_INS(0xfa, "JM"      , 10, 3, _fa_jm_a16);
    ADD_INS(0xfc, "CM"      , 17, 3, _fc_cm_a16);
    // ADD_INS(0xfd, "*CALL"   , 17, 3, _fd_call_a16_alt);
    ADD_INS(0xff, "RST_7"   , 11, 1, _ff_rst_7);

    // +++++++ Jump/Calls instructions

    // +++++++ START: Misc instructions
    ADD_INS(0x23, "INC HL"      , 5 , 1, _23_inx_h_d8);
    // +++++++ END: Misc instructions


    // +++++++ START: 8 bits load/store instructions
    ADD_INS(0x02, "STAX B"      , 7 , 1, _02_stax_b_d8);
    ADD_INS(0x06, "MVI B"       , 7 , 2, _06_mvi_b_d8);
    ADD_INS(0x0a, "LDAX B"      , 7 , 1, _0a_ldax_b_d8);
    ADD_INS(0x0e, "MVI C"       , 7 , 2, _0e_mvi_d_d8);

    ADD_INS(0x12, "STAX D"      , 7 , 1, _12_stax_d_d8);
    ADD_INS(0x16, "MVI D"       , 7 , 2, _16_mvi_d_d8);
    ADD_INS(0x1a, "LDAX D"      , 7 , 1, _1a_ldax_d_d8);
    ADD_INS(0x1e, "MVI E"       , 7 , 2, _1e_mvi_e_d8);

    ADD_INS(0x26, "MVI H"       , 7 , 2, _26_mvi_h_d8);
    ADD_INS(0x2e, "MVI L"       , 7 , 2, _2e_mvi_l_d8);

    ADD_INS(0x32, "STA A16"     , 13, 3, _32_sta_a16_d16);
    ADD_INS(0x36, "MVI M"       , 10, 2, _36_mvi_m_d8);
    ADD_INS(0x3a, "LDA A16"     , 13, 3, _3a_lda_a16_d16);
    ADD_INS(0x3e, "MVI A"       , 7 , 2, _3e_mvi_a_d8);

    ADD_INS(0x40, "MOV B,B"     , 5 , 1, _40_mov_b_b_d8);
    ADD_INS(0x41, "MOV B,C"     , 5 , 1, _41_mov_b_c_d8);
    ADD_INS(0x42, "MOV B,D"     , 5 , 1, _42_mov_b_d_d8);
    ADD_INS(0x43, "MOV B,E"     , 5 , 1, _43_mov_b_e_d8);
    ADD_INS(0x44, "MOV B,H"     , 5 , 1, _44_mov_b_h_d8);
    ADD_INS(0x45, "MOV B,L"     , 5 , 1, _45_mov_b_l_d8);
    ADD_INS(0x46, "MOV B,M"     , 7 , 1, _46_mov_b_m_d8);
    ADD_INS(0x47, "MOV B,A"     , 5 , 1, _47_mov_b_a_d8);

    ADD_INS(0x48, "MOV C,B"     , 5 , 1, _48_mov_c_b_d8);
    ADD_INS(0x49, "MOV C,C"     , 5 , 1, _49_mov_c_c_d8);
    ADD_INS(0x4a, "MOV C,D"     , 5 , 1, _4a_mov_c_d_d8);
    ADD_INS(0x4b, "MOV C,E"     , 5 , 1, _4b_mov_c_e_d8);
    ADD_INS(0x4c, "MOV C,H"     , 5 , 1, _4c_mov_c_h_d8);
    ADD_INS(0x4d, "MOV C,L"     , 5 , 1, _4d_mov_c_l_d8);
    ADD_INS(0x4e, "MOV C,M"     , 7 , 1, _4e_mov_c_m_d8);
    ADD_INS(0x4f, "MOV C,A"     , 5 , 1, _4f_mov_c_a_d8);

    ADD_INS(0x50, "MOV D,B"     , 5 , 1, _50_mov_d_b_d8);
    ADD_INS(0x51, "MOV D,C"     , 5 , 1, _51_mov_d_c_d8);
    ADD_INS(0x52, "MOV D,D"     , 5 , 1, _52_mov_d_d_d8);
    ADD_INS(0x53, "MOV D,E"     , 5 , 1, _53_mov_d_e_d8);
    ADD_INS(0x54, "MOV D,H"     , 5 , 1, _54_mov_d_h_d8);
    ADD_INS(0x55, "MOV D,L"     , 5 , 1, _55_mov_d_l_d8);
    ADD_INS(0x56, "MOV D,M"     , 7 , 1, _56_mov_d_m_d8);
    ADD_INS(0x57, "MOV D,A"     , 5 , 1, _57_mov_d_a_d8);

    ADD_INS(0x58, "MOV E,B"     , 5 , 1, _58_mov_e_b_d8);
    ADD_INS(0x59, "MOV E,C"     , 5 , 1, _59_mov_e_c_d8);
    ADD_INS(0x5a, "MOV E,D"     , 5 , 1, _5a_mov_e_d_d8);
    ADD_INS(0x5b, "MOV E,E"     , 5 , 1, _5b_mov_e_e_d8);
    ADD_INS(0x5c, "MOV E,H"     , 5 , 1, _5c_mov_e_h_d8);
    ADD_INS(0x5d, "MOV E,L"     , 5 , 1, _5d_mov_e_l_d8);
    ADD_INS(0x5e, "MOV E,M"     , 7 , 1, _5e_mov_e_m_d8);
    ADD_INS(0x5f, "MOV E,A"     , 5 , 1, _5f_mov_e_a_d8);

    ADD_INS(0x60, "MOV H,B"     , 5 , 1, _60_mov_h_b_d8);
    ADD_INS(0x61, "MOV H,C"     , 5 , 1, _61_mov_h_c_d8);
    ADD_INS(0x62, "MOV H,D"     , 5 , 1, _62_mov_h_d_d8);
    ADD_INS(0x63, "MOV H,E"     , 5 , 1, _63_mov_h_e_d8);
    ADD_INS(0x64, "MOV H,H"     , 5 , 1, _64_mov_h_h_d8);
    ADD_INS(0x65, "MOV H,L"     , 5 , 1, _65_mov_h_l_d8);
    ADD_INS(0x66, "MOV H,M"     , 7 , 1, _66_mov_h_m_d8);
    ADD_INS(0x67, "MOV H,A"     , 5 , 1, _67_mov_h_a_d8);

    ADD_INS(0x68, "MOV L,B"     , 5 , 1, _68_mov_l_b_d8);
    ADD_INS(0x69, "MOV L,C"     , 5 , 1, _69_mov_l_c_d8);
    ADD_INS(0x6a, "MOV L,D"     , 5 , 1, _6a_mov_l_d_d8);
    ADD_INS(0x6b, "MOV L,E"     , 5 , 1, _6b_mov_l_e_d8);
    ADD_INS(0x6c, "MOV L,H"     , 5 , 1, _6c_mov_l_h_d8);
    ADD_INS(0x6d, "MOV L,L"     , 5 , 1, _6d_mov_l_l_d8);
    ADD_INS(0x6e, "MOV L,M"     , 7 , 1, _6e_mov_l_m_d8);
    ADD_INS(0x6f, "MOV L,A"     , 5 , 1, _6f_mov_l_a_d8);

    ADD_INS(0x70, "MOV M,B"     , 7 , 1, _70_mov_m_b_d8);
    ADD_INS(0x71, "MOV M,C"     , 7 , 1, _71_mov_m_c_d8);
    ADD_INS(0x72, "MOV M,D"     , 7 , 1, _72_mov_m_d_d8);
    ADD_INS(0x73, "MOV M,E"     , 7 , 1, _73_mov_m_e_d8);
    ADD_INS(0x74, "MOV M,H"     , 7 , 1, _74_mov_m_h_d8);
    ADD_INS(0x75, "MOV M,L"     , 7 , 1, _75_mov_m_l_d8);
    ADD_INS(0x76, "HALT", 7, 1, [this](uint8_t * opcode, cpu_state_t& state) {
        printf("FUCK YOU!!!\n");
        throw std::system_error(EFAULT, std::generic_category());
    });
    ADD_INS(0x77, "MOV M,A"     , 7 , 1, _77_mov_m_a_d8);

    ADD_INS(0x78, "MOV A,B"     , 5 , 1, _78_mov_a_b_d8);
    ADD_INS(0x79, "MOV A,C"     , 5 , 1, _79_mov_a_c_d8);
    ADD_INS(0x7a, "MOV A,D"     , 5 , 1, _7a_mov_a_d_d8);
    ADD_INS(0x7b, "MOV A,E"     , 5 , 1, _7b_mov_a_e_d8);
    ADD_INS(0x7c, "MOV A,H"     , 5 , 1, _7c_mov_a_h_d8);
    ADD_INS(0x7d, "MOV A,L"     , 5 , 1, _7d_mov_a_l_d8);
    ADD_INS(0x7e, "MOV A,M"     , 7 , 1, _7e_mov_a_m_d8);
    ADD_INS(0x7f, "MOV A,A"     , 5 , 1, _7f_mov_a_a_d8);

    // +++++++ END: 16 bits load/store instructions

    // +++++++ START: 16 bits load/store instructions
    ADD_INS(0x01, "LXI B", 10, 3, _01_lxi_b_d16);
    ADD_INS(0x11, "LXI D", 10, 3, _11_lxi_d_d16);
    ADD_INS(0x21, "LXI H", 10, 3, _21_lxi_h_d16);
    ADD_INS(0x22, "SHLD" , 16, 3, _22_shld_a16);
    ADD_INS(0x2a, "LHLD" , 16, 3, _2a_lhld_a16);
    ADD_INS(0x31, "LD SP", 10, 3, _31_lxi_sp_d16);

    ADD_INS(0xC1, "POP B", 10, 1, _c1_pop_b_d16);
    ADD_INS(0xC5, "PUSH B", 11, 1, _c5_push_b_d16);

    ADD_INS(0xD1, "POP D", 10, 1, _d1_pop_d_d16);
    ADD_INS(0xD5, "PUSH D", 11, 1, _d5_push_d_d16);

    ADD_INS(0xE1, "POP H", 10, 1, _e1_pop_h_d16);
    ADD_INS(0xE3, "XTHL" , 10, 1, _e3_xhtl_d16);
    ADD_INS(0xE5, "PUSH H", 11, 1, _e5_push_h_d16);
    ADD_INS(0xEB, "XCHG", 5, 1, _eb_xchg_d16);

    ADD_INS(0xF1, "POP PSW", 10, 1, _f1_pop_psw_d16);
    ADD_INS(0xF5, "PUSH PSW", 11, 1, _f5_push_psw_d16);
    ADD_INS(0xF9, "SPHL", 5, 1, _f9_sphl_d16);
    // +++++++ END: 16 bits load/store instructions

    // +++++++ START: 8 bits arithmetic/logical instructions
    ADD_INS(0x04, "INR B", 5, 1, _04_inr_b);
    ADD_INS(0x0c, "INR C", 5, 1, _0c_inr_c);
    ADD_INS(0x14, "INR D", 5, 1, _14_inr_d);
    ADD_INS(0x1c, "INR E", 5, 1, _1c_inr_e);
    ADD_INS(0x24, "INR H", 5, 1, _24_inr_h);
    ADD_INS(0x2c, "INR L", 5, 1, _2c_inr_l);
    ADD_INS(0x34, "INR M", 5, 1, _34_inr_m);
    ADD_INS(0x3c, "INR A", 5, 1, _3c_inr_a);

    ADD_INS(0x05, "DCR B", 5, 1, _05_dcr_b);
    ADD_INS(0x0d, "DCR C", 5, 1, _0d_dcr_c);
    ADD_INS(0x15, "DCR D", 5, 1, _15_dcr_d);
    ADD_INS(0x1d, "DCR E", 5, 1, _1d_dcr_e);
    ADD_INS(0x25, "DCR H", 5, 1, _25_dcr_h);
    ADD_INS(0x2d, "DCR L", 5, 1, _2d_dcr_l);
    ADD_INS(0x35, "DCR M", 5, 1, _35_dcr_m);
    ADD_INS(0x3d, "DCR A", 5, 1, _3d_dcr_a);

    ADD_INS(0x07, "RLC", 5, 1, _07_rlc);
    ADD_INS(0x0f, "RRC", 5, 1, _0f_rrc);
    ADD_INS(0x17, "RAL", 5, 1, _17_ral);
    ADD_INS(0x1f, "RAR", 5, 1, _1f_rar);
    ADD_INS(0x27, "DAA", 5, 1, _27_daa);
    ADD_INS(0x2f, "CMA", 5, 1, _2f_cma);
    ADD_INS(0x37, "STC", 5, 1, _37_stc);
    ADD_INS(0x3f, "CMC", 5, 1, _3f_cmc);

    ADD_INS(0x80, "ADD B"       , 4 , 1, _80_add_b);
    ADD_INS(0x81, "ADD C"       , 4 , 1, _81_add_c);
    ADD_INS(0x82, "ADD D"       , 4 , 1, _82_add_d);
    ADD_INS(0x83, "ADD E"       , 4 , 1, _83_add_e);
    ADD_INS(0x84, "ADD H"       , 4 , 1, _84_add_h);
    ADD_INS(0x85, "ADD L"       , 4 , 1, _85_add_l);
    ADD_INS(0x86, "ADD M"       , 7 , 1, _86_add_m);
    ADD_INS(0x87, "ADD A"       , 4 , 1, _87_add_a);

    ADD_INS(0x88, "ADC B"       , 4 , 1, _88_adc_b);
    ADD_INS(0x89, "ADC C"       , 4 , 1, _89_adc_c);
    ADD_INS(0x8a, "ADC D"       , 4 , 1, _8a_adc_d);
    ADD_INS(0x8b, "ADC E"       , 4 , 1, _8b_adc_e);
    ADD_INS(0x8c, "ADC H"       , 4 , 1, _8c_adc_h);
    ADD_INS(0x8d, "ADC L"       , 4 , 1, _8d_adc_l);
    ADD_INS(0x8e, "ADC M"       , 7 , 1, _8e_adc_m);
    ADD_INS(0x8f, "ADC A"       , 4 , 1, _8f_adc_a);

    ADD_INS(0x90, "SUB B"       , 4 , 1, _90_sub_b);
    ADD_INS(0x91, "SUB C"       , 4 , 1, _91_sub_c);
    ADD_INS(0x92, "SUB D"       , 4 , 1, _92_sub_d);
    ADD_INS(0x93, "SUB E"       , 4 , 1, _93_sub_e);
    ADD_INS(0x94, "SUB H"       , 4 , 1, _94_sub_h);
    ADD_INS(0x95, "SUB L"       , 4 , 1, _95_sub_l);
    ADD_INS(0x96, "SUB M"       , 7 , 1, _96_sub_m);
    ADD_INS(0x97, "SUB A"       , 4 , 1, _97_sub_a);

    ADD_INS(0x98, "SBB B"       , 4 , 1, _98_sbb_b);
    ADD_INS(0x99, "SBB C"       , 4 , 1, _99_sbb_c);
    ADD_INS(0x9a, "SBB D"       , 4 , 1, _9a_sbb_d);
    ADD_INS(0x9b, "SBB E"       , 4 , 1, _9b_sbb_e);
    ADD_INS(0x9c, "SBB H"       , 4 , 1, _9c_sbb_h);
    ADD_INS(0x9d, "SBB L"       , 4 , 1, _9d_sbb_l);
    ADD_INS(0x9e, "SBB M"       , 7 , 1, _9e_sbb_m);
    ADD_INS(0x9f, "SBB A"       , 4 , 1, _9f_sbb_a);

    ADD_INS(0xA0, "ANA B"       , 4 , 1, _a0_ana_b);
    ADD_INS(0xA1, "ANA C"       , 4 , 1, _a1_ana_c);
    ADD_INS(0xA2, "ANA D"       , 4 , 1, _a2_ana_d);
    ADD_INS(0xA3, "ANA E"       , 4 , 1, _a3_ana_e);
    ADD_INS(0xA4, "ANA H"       , 4 , 1, _a4_ana_h);
    ADD_INS(0xA5, "ANA L"       , 4 , 1, _a5_ana_l);
    ADD_INS(0xA6, "ANA M"       , 7 , 1, _a6_ana_m);
    ADD_INS(0xA7, "ANA A"       , 4 , 1, _a7_ana_a);

    ADD_INS(0xA8, "XRA B"       , 4 , 1, _a8_xra_b);
    ADD_INS(0xA9, "XRA C"       , 4 , 1, _a9_xra_c);
    ADD_INS(0xAa, "XRA D"       , 4 , 1, _aa_xra_d);
    ADD_INS(0xAb, "XRA E"       , 4 , 1, _ab_xra_e);
    ADD_INS(0xAc, "XRA H"       , 4 , 1, _ac_xra_h);
    ADD_INS(0xAd, "XRA L"       , 4 , 1, _ad_xra_l);
    ADD_INS(0xAe, "XRA M"       , 7 , 1, _ae_xra_m);
    ADD_INS(0xAf, "XRA A"       , 4 , 1, _af_xra_a);

    ADD_INS(0xB0, "ORA B"       , 4 , 1, _b0_ora_b);
    ADD_INS(0xB1, "ORA C"       , 4 , 1, _b1_ora_c);
    ADD_INS(0xB2, "ORA D"       , 4 , 1, _b2_ora_d);
    ADD_INS(0xB3, "ORA E"       , 4 , 1, _b3_ora_e);
    ADD_INS(0xB4, "ORA H"       , 4 , 1, _b4_ora_h);
    ADD_INS(0xB5, "ORA L"       , 4 , 1, _b5_ora_l);
    ADD_INS(0xB6, "ORA M"       , 7 , 1, _b6_ora_m);
    ADD_INS(0xB7, "ORA A"       , 4 , 1, _b7_ora_a);

    ADD_INS(0xb8, "CMP B"       , 4 , 1, _b8_cmp_b);
    ADD_INS(0xb9, "CMP C"       , 4 , 1, _b9_cmp_c);
    ADD_INS(0xba, "CMP D"       , 4 , 1, _ba_cmp_d);
    ADD_INS(0xbb, "CMP E"       , 4 , 1, _bb_cmp_e);
    ADD_INS(0xbc, "CMP H"       , 4 , 1, _bc_cmp_h);
    ADD_INS(0xbd, "CMP L"       , 4 , 1, _bd_cmp_l);
    ADD_INS(0xbe, "CMP M"       , 7 , 1, _be_cmp_m);
    ADD_INS(0xbf, "CMP A"       , 4 , 1, _bf_cmp_a);

    ADD_INS(0xc6, "ADI", 7, 2, _c6_adi_d8);
    ADD_INS(0xce, "ACI", 7, 2, _ce_aci_d8);
    ADD_INS(0xd6, "SUI", 7, 2, _d6_sui_d8);
    ADD_INS(0xde, "SBI", 7, 2, _de_sbi_d8);
    ADD_INS(0xe6, "ANI", 7, 2, _e6_ani_d8);
    ADD_INS(0xee, "XRI", 7, 2, _ee_xri_d8);
    ADD_INS(0xf6, "ORI", 7, 2, _f6_ori_d8);
    ADD_INS(0xfe, "CPI", 7, 2, _fe_cpi_d8);
    // +++++++ END: 8 bits arithmetic/logical instructions

    // +++++++ START: 16 bits arithmetic/logical instructions
    ADD_INS(0x03, "INX B", 5, 1, _03_inx_b_d16);
    ADD_INS(0x09, "DAD B", 5, 1, _09_dad_b_d16);
    ADD_INS(0x0b, "DCX B", 5, 1, _0b_dcx_b_d16);
    ADD_INS(0x13, "INX D", 5, 1, _13_inx_d_d16);
    ADD_INS(0x19, "DAD D", 5, 1, _19_dad_d_d16);
    ADD_INS(0x1b, "DCX D", 5, 1, _1b_dcx_d_d16);
    ADD_INS(0x23, "INX H", 5, 1, _23_inx_h_d16);
    ADD_INS(0x29, "DAD H", 5, 1, _29_dad_h_d16);
    ADD_INS(0x2b, "DCX H", 5, 1, _2b_dcx_h_d16);
    ADD_INS(0x33, "INC SP", 5, 1, _33_inx_sp_d16);
    ADD_INS(0x39, "DAD SP", 5, 1, _39_dad_sp_d16);
    ADD_INS(0x3b, "DCX SP", 5, 1, _3b_dcx_sp_d16);
    // +++++++ END: 16 bits arithmetic/logical instructions

    for (int i=0; i<255; ++i) {
        if (this->instructions[i].name == "ERROR") {
            std::cout << hex << setfill('0') << setw(2) << (int) this->instructions[i].id << " - " << this->instructions[i].name << std::endl;
        }
    }
}