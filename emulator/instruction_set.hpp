#pragma once

// struct cpu_state_t;
#include "cpu_state.hpp"

void _f3_di(uint8_t * opcode, cpu_state_t& state);
void _fb_ei(uint8_t * opcode, cpu_state_t& state);

void _c0_rnz(uint8_t * opcode, cpu_state_t& state);
void _c2_jnz_a16(uint8_t * opcode, cpu_state_t& state);
void _c3_jmp_a16(uint8_t * opcode, cpu_state_t& state);
void _c4_cnz_a16(uint8_t * opcode, cpu_state_t& state);
void _c7_rst_0(uint8_t * opcode, cpu_state_t& state);
void _c8_rz(uint8_t * opcode, cpu_state_t& state);
void _c9_ret(uint8_t * opcode, cpu_state_t& state);
void _ca_jz_a16(uint8_t * opcode, cpu_state_t& state);
void _cb_jmp_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _cc_cz_a16(uint8_t * opcode, cpu_state_t& state);
void _cd_call_a16(uint8_t * opcode, cpu_state_t& state);
void _cf_rst_1(uint8_t * opcode, cpu_state_t& state);

void _d0_rnc(uint8_t * opcode, cpu_state_t& state);
void _d2_jnc_a16(uint8_t * opcode, cpu_state_t& state);
void _d4_cnc_a16(uint8_t * opcode, cpu_state_t& state);
void _d7_rst_2(uint8_t * opcode, cpu_state_t& state);
void _d8_rc(uint8_t * opcode, cpu_state_t& state);
void _d9_ret_alt(uint8_t * opcode, cpu_state_t& state);
void _da_jc_a16(uint8_t * opcode, cpu_state_t& state);
void _dc_cc_a16(uint8_t * opcode, cpu_state_t& state);
void _dd_call_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _df_rst_3(uint8_t * opcode, cpu_state_t& state);

void _e0_rpo(uint8_t * opcode, cpu_state_t& state);
void _e2_jpo_a16(uint8_t * opcode, cpu_state_t& state);
void _e4_cpo_a16(uint8_t * opcode, cpu_state_t& state);
void _e7_rst_4(uint8_t * opcode, cpu_state_t& state);
void _e8_rpe(uint8_t * opcode, cpu_state_t& state);
void _e9_phcl(uint8_t * opcode, cpu_state_t& state);
void _ea_jpe_a16(uint8_t * opcode, cpu_state_t& state);
void _ec_cpe_a16(uint8_t * opcode, cpu_state_t& state);
void _ed_call_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _ef_rst_5(uint8_t * opcode, cpu_state_t& state);

void _f0_rp(uint8_t * opcode, cpu_state_t& state);
void _f2_jp_a16(uint8_t * opcode, cpu_state_t& state);
void _f4_cp_a16(uint8_t * opcode, cpu_state_t& state);
void _f7_rst_6(uint8_t * opcode, cpu_state_t& state);
void _f8_rm(uint8_t * opcode, cpu_state_t& state);
void _fa_jm_a16(uint8_t * opcode, cpu_state_t& state);
void _fc_cm_a16(uint8_t * opcode, cpu_state_t& state);
void _fd_call_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _ff_rst_7(uint8_t * opcode, cpu_state_t& state);

// +++++++ START: 8 bits load/store instructions
void _06_mvi_b_d8(uint8_t * opcode, cpu_state_t& state);
void _1a_ldax_d_d8(uint8_t * opcode, cpu_state_t& state);
void _23_inx_h_d8(uint8_t * opcode, cpu_state_t& state);
void _77_mov_m_a_d8(int8_t * opcode, cpu_state_t& state);
// +++++++ END: 16 bits load/store instructions

// +++++++ START: 16 bits load/store instructions
void _11_lxi_d_d16(uint8_t * opcode, cpu_state_t& state);
void _21_lxi_h_d16(uint8_t * opcode, cpu_state_t& state);
void _31_lxi_sp_d16(uint8_t * opcode, cpu_state_t& state);
// +++++++ END: 16 bits load/store instructions

// +++++++ START: 8 bits arithmetic/logical instructions
void _05_dcr_b(uint8_t * opcode, cpu_state_t& state);
void _c6_adi_d8(uint8_t * opcode, cpu_state_t& state);
void _e6_ani_d8(uint8_t * opcode, cpu_state_t& state);
// +++++++ END: 8 bits arithmetic/logical instructions

// +++++++ START: 16 bits arithmetic/logical instructions
void _13_inx_d_d16(uint8_t * opcode, cpu_state_t& state);
// +++++++ END: 16 bits arithmetic/logical instructions
