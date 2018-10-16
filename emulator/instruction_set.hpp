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
// void _cb_jmp_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _cc_cz_a16(uint8_t * opcode, cpu_state_t& state);
void _cd_call_a16(uint8_t * opcode, cpu_state_t& state);
void _cf_rst_1(uint8_t * opcode, cpu_state_t& state);

void _d0_rnc(uint8_t * opcode, cpu_state_t& state);
void _d2_jnc_a16(uint8_t * opcode, cpu_state_t& state);
void _d4_cnc_a16(uint8_t * opcode, cpu_state_t& state);
void _d7_rst_2(uint8_t * opcode, cpu_state_t& state);
void _d8_rc(uint8_t * opcode, cpu_state_t& state);
//void _d9_ret_alt(uint8_t * opcode, cpu_state_t& state);
void _da_jc_a16(uint8_t * opcode, cpu_state_t& state);
void _dc_cc_a16(uint8_t * opcode, cpu_state_t& state);
// void _dd_call_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _df_rst_3(uint8_t * opcode, cpu_state_t& state);

void _e0_rpo(uint8_t * opcode, cpu_state_t& state);
void _e2_jpo_a16(uint8_t * opcode, cpu_state_t& state);
void _e4_cpo_a16(uint8_t * opcode, cpu_state_t& state);
void _e7_rst_4(uint8_t * opcode, cpu_state_t& state);
void _e8_rpe(uint8_t * opcode, cpu_state_t& state);
void _e9_phcl(uint8_t * opcode, cpu_state_t& state);
void _ea_jpe_a16(uint8_t * opcode, cpu_state_t& state);
void _ec_cpe_a16(uint8_t * opcode, cpu_state_t& state);
// void _ed_call_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _ef_rst_5(uint8_t * opcode, cpu_state_t& state);

void _f0_rp(uint8_t * opcode, cpu_state_t& state);
void _f2_jp_a16(uint8_t * opcode, cpu_state_t& state);
void _f4_cp_a16(uint8_t * opcode, cpu_state_t& state);
void _f7_rst_6(uint8_t * opcode, cpu_state_t& state);
void _f8_rm(uint8_t * opcode, cpu_state_t& state);
void _fa_jm_a16(uint8_t * opcode, cpu_state_t& state);
void _fc_cm_a16(uint8_t * opcode, cpu_state_t& state);
// void _fd_call_a16_alt(uint8_t * opcode, cpu_state_t& state);
void _ff_rst_7(uint8_t * opcode, cpu_state_t& state);

// +++++++ START: 8 bits load/store instructions
void _02_stax_b_d8(uint8_t * opcode, cpu_state_t& state);
void _06_mvi_b_d8(uint8_t * opcode, cpu_state_t& state);
void _0a_ldax_b_d8(uint8_t * opcode, cpu_state_t& state);
void _0e_mvi_d_d8(uint8_t * opcode, cpu_state_t& state);

void _12_stax_d_d8(uint8_t * opcode, cpu_state_t& state);
void _16_mvi_d_d8(uint8_t * opcode, cpu_state_t& state);
void _1a_ldax_d_d8(uint8_t * opcode, cpu_state_t& state);
void _1e_mvi_e_d8(uint8_t * opcode, cpu_state_t& state);

void _26_mvi_h_d8(uint8_t * opcode, cpu_state_t& state);
void _2e_mvi_l_d8(uint8_t * opcode, cpu_state_t& state);

void _32_sta_a16_d16(uint8_t * opcode, cpu_state_t& state);
void _36_mvi_m_d8(uint8_t * opcode, cpu_state_t& state);
void _3a_lda_a16_d16(uint8_t * opcode, cpu_state_t& state);
void _3e_mvi_a_d8(uint8_t * opcode, cpu_state_t& state);

void _40_mov_b_b_d8(uint8_t * opcode, cpu_state_t& state);
void _41_mov_b_c_d8(uint8_t * opcode, cpu_state_t& state);
void _42_mov_b_d_d8(uint8_t * opcode, cpu_state_t& state);
void _43_mov_b_e_d8(uint8_t * opcode, cpu_state_t& state);
void _44_mov_b_h_d8(uint8_t * opcode, cpu_state_t& state);
void _45_mov_b_l_d8(uint8_t * opcode, cpu_state_t& state);
void _46_mov_b_m_d8(uint8_t * opcode, cpu_state_t& state);
void _47_mov_b_a_d8(uint8_t * opcode, cpu_state_t& state);

void _48_mov_c_b_d8(uint8_t * opcode, cpu_state_t& state);
void _49_mov_c_c_d8(uint8_t * opcode, cpu_state_t& state);
void _4a_mov_c_d_d8(uint8_t * opcode, cpu_state_t& state);
void _4b_mov_c_e_d8(uint8_t * opcode, cpu_state_t& state);
void _4c_mov_c_h_d8(uint8_t * opcode, cpu_state_t& state);
void _4d_mov_c_l_d8(uint8_t * opcode, cpu_state_t& state);
void _4e_mov_c_m_d8(uint8_t * opcode, cpu_state_t& state);
void _4f_mov_c_a_d8(uint8_t * opcode, cpu_state_t& state);

void _50_mov_d_b_d8(uint8_t * opcode, cpu_state_t& state);
void _51_mov_d_c_d8(uint8_t * opcode, cpu_state_t& state);
void _52_mov_d_d_d8(uint8_t * opcode, cpu_state_t& state);
void _53_mov_d_e_d8(uint8_t * opcode, cpu_state_t& state);
void _54_mov_d_h_d8(uint8_t * opcode, cpu_state_t& state);
void _55_mov_d_l_d8(uint8_t * opcode, cpu_state_t& state);
void _56_mov_d_m_d8(uint8_t * opcode, cpu_state_t& state);
void _57_mov_d_a_d8(uint8_t * opcode, cpu_state_t& state);

void _58_mov_e_b_d8(uint8_t * opcode, cpu_state_t& state);
void _59_mov_e_c_d8(uint8_t * opcode, cpu_state_t& state);
void _5a_mov_e_d_d8(uint8_t * opcode, cpu_state_t& state);
void _5b_mov_e_e_d8(uint8_t * opcode, cpu_state_t& state);
void _5c_mov_e_h_d8(uint8_t * opcode, cpu_state_t& state);
void _5d_mov_e_l_d8(uint8_t * opcode, cpu_state_t& state);
void _5e_mov_e_m_d8(uint8_t * opcode, cpu_state_t& state);
void _5f_mov_e_a_d8(uint8_t * opcode, cpu_state_t& state);

void _60_mov_h_b_d8(uint8_t * opcode, cpu_state_t& state);
void _61_mov_h_c_d8(uint8_t * opcode, cpu_state_t& state);
void _62_mov_h_d_d8(uint8_t * opcode, cpu_state_t& state);
void _63_mov_h_e_d8(uint8_t * opcode, cpu_state_t& state);
void _64_mov_h_h_d8(uint8_t * opcode, cpu_state_t& state);
void _65_mov_h_l_d8(uint8_t * opcode, cpu_state_t& state);
void _66_mov_h_m_d8(uint8_t * opcode, cpu_state_t& state);
void _67_mov_h_a_d8(uint8_t * opcode, cpu_state_t& state);

void _68_mov_l_b_d8(uint8_t * opcode, cpu_state_t& state);
void _69_mov_l_c_d8(uint8_t * opcode, cpu_state_t& state);
void _6a_mov_l_d_d8(uint8_t * opcode, cpu_state_t& state);
void _6b_mov_l_e_d8(uint8_t * opcode, cpu_state_t& state);
void _6c_mov_l_h_d8(uint8_t * opcode, cpu_state_t& state);
void _6d_mov_l_l_d8(uint8_t * opcode, cpu_state_t& state);
void _6e_mov_l_m_d8(uint8_t * opcode, cpu_state_t& state);
void _6f_mov_l_a_d8(uint8_t * opcode, cpu_state_t& state);

void _70_mov_m_b_d8(uint8_t * opcode, cpu_state_t& state);
void _71_mov_m_c_d8(uint8_t * opcode, cpu_state_t& state);
void _72_mov_m_d_d8(uint8_t * opcode, cpu_state_t& state);
void _73_mov_m_e_d8(uint8_t * opcode, cpu_state_t& state);
void _74_mov_m_h_d8(uint8_t * opcode, cpu_state_t& state);
void _75_mov_m_l_d8(uint8_t * opcode, cpu_state_t& state);
void _77_mov_m_a_d8(uint8_t * opcode, cpu_state_t& state);

void _78_mov_a_b_d8(uint8_t * opcode, cpu_state_t& state);
void _79_mov_a_c_d8(uint8_t * opcode, cpu_state_t& state);
void _7a_mov_a_d_d8(uint8_t * opcode, cpu_state_t& state);
void _7b_mov_a_e_d8(uint8_t * opcode, cpu_state_t& state);
void _7c_mov_a_h_d8(uint8_t * opcode, cpu_state_t& state);
void _7d_mov_a_l_d8(uint8_t * opcode, cpu_state_t& state);
void _7e_mov_a_m_d8(uint8_t * opcode, cpu_state_t& state);
void _7f_mov_a_a_d8(uint8_t * opcode, cpu_state_t& state);
// +++++++ END: 8 bits load/store instructions

// +++++++ START: 16 bits load/store instructions
void _01_lxi_b_d16(uint8_t * opcode, cpu_state_t& state);
void _11_lxi_d_d16(uint8_t * opcode, cpu_state_t& state);
void _21_lxi_h_d16(uint8_t * opcode, cpu_state_t& state);
void _22_shld_a16(uint8_t * opcode, cpu_state_t& state);
void _2a_lhld_a16(uint8_t * opcode, cpu_state_t& state);
void _31_lxi_sp_d16(uint8_t * opcode, cpu_state_t& state);

void _c1_pop_b_d16(uint8_t * opcode, cpu_state_t& state);
void _c5_push_b_d16(uint8_t * opcode, cpu_state_t& state);
void _d1_pop_d_d16(uint8_t * opcode, cpu_state_t& state);
void _d5_push_d_d16(uint8_t * opcode, cpu_state_t& state);

void _e1_pop_h_d16(uint8_t * opcode, cpu_state_t& state);
void _e3_xhtl_d16(uint8_t * opcode, cpu_state_t& state);
void _e5_push_h_d16(uint8_t * opcode, cpu_state_t& state);
void _eb_xchg_d16(uint8_t * opcode, cpu_state_t& state);

void _f1_pop_psw_d16(uint8_t * opcode, cpu_state_t& state);
void _f5_push_psw_d16(uint8_t * opcode, cpu_state_t& state);
void _f9_sphl_d16(uint8_t * opcode, cpu_state_t& state);
// +++++++ END: 16 bits load/store instructions

// +++++++ START: 8 bits arithmetic/logical instructions
void _04_inr_b(uint8_t * opcode, cpu_state_t& state);
void _0c_inr_c(uint8_t * opcode, cpu_state_t& state);
void _14_inr_d(uint8_t * opcode, cpu_state_t& state);
void _1c_inr_e(uint8_t * opcode, cpu_state_t& state);
void _24_inr_h(uint8_t * opcode, cpu_state_t& state);
void _2c_inr_l(uint8_t * opcode, cpu_state_t& state);
void _34_inr_m(uint8_t * opcode, cpu_state_t& state);
void _3c_inr_a(uint8_t * opcode, cpu_state_t& state);

void _05_dcr_b(uint8_t * opcode, cpu_state_t& state);
void _0d_dcr_c(uint8_t * opcode, cpu_state_t& state);
void _15_dcr_d(uint8_t * opcode, cpu_state_t& state);
void _1d_dcr_e(uint8_t * opcode, cpu_state_t& state);
void _25_dcr_h(uint8_t * opcode, cpu_state_t& state);
void _2d_dcr_l(uint8_t * opcode, cpu_state_t& state);
void _35_dcr_m(uint8_t * opcode, cpu_state_t& state);
void _3d_dcr_a(uint8_t * opcode, cpu_state_t& state);

void _07_rlc(uint8_t * opcode, cpu_state_t& state);
void _0f_rrc(uint8_t * opcode, cpu_state_t& state);
void _17_ral(uint8_t * opcode, cpu_state_t& state);
void _1f_rar(uint8_t * opcode, cpu_state_t& state);
void _27_daa(uint8_t * opcode, cpu_state_t& state);
void _2f_cma(uint8_t * opcode, cpu_state_t& state);
void _37_stc(uint8_t * opcode, cpu_state_t& state);
void _3f_cmc(uint8_t * opcode, cpu_state_t& state);

void _80_add_b(uint8_t * opcode, cpu_state_t& state);
void _81_add_c(uint8_t * opcode, cpu_state_t& state);
void _82_add_d(uint8_t * opcode, cpu_state_t& state);
void _83_add_e(uint8_t * opcode, cpu_state_t& state);
void _84_add_h(uint8_t * opcode, cpu_state_t& state);
void _85_add_l(uint8_t * opcode, cpu_state_t& state);
void _86_add_m(uint8_t * opcode, cpu_state_t& state);
void _87_add_a(uint8_t * opcode, cpu_state_t& state);

void _88_adc_b(uint8_t * opcode, cpu_state_t& state);
void _89_adc_c(uint8_t * opcode, cpu_state_t& state);
void _8a_adc_d(uint8_t * opcode, cpu_state_t& state);
void _8b_adc_e(uint8_t * opcode, cpu_state_t& state);
void _8c_adc_h(uint8_t * opcode, cpu_state_t& state);
void _8d_adc_l(uint8_t * opcode, cpu_state_t& state);
void _8e_adc_m(uint8_t * opcode, cpu_state_t& state);
void _8f_adc_a(uint8_t * opcode, cpu_state_t& state);

void _90_sub_b(uint8_t * opcode, cpu_state_t& state);
void _91_sub_c(uint8_t * opcode, cpu_state_t& state);
void _92_sub_d(uint8_t * opcode, cpu_state_t& state);
void _93_sub_e(uint8_t * opcode, cpu_state_t& state);
void _94_sub_h(uint8_t * opcode, cpu_state_t& state);
void _95_sub_l(uint8_t * opcode, cpu_state_t& state);
void _96_sub_m(uint8_t * opcode, cpu_state_t& state);
void _97_sub_a(uint8_t * opcode, cpu_state_t& state);

void _98_sbb_b(uint8_t * opcode, cpu_state_t& state);
void _99_sbb_c(uint8_t * opcode, cpu_state_t& state);
void _9a_sbb_d(uint8_t * opcode, cpu_state_t& state);
void _9b_sbb_e(uint8_t * opcode, cpu_state_t& state);
void _9c_sbb_h(uint8_t * opcode, cpu_state_t& state);
void _9d_sbb_l(uint8_t * opcode, cpu_state_t& state);
void _9e_sbb_m(uint8_t * opcode, cpu_state_t& state);
void _9f_sbb_a(uint8_t * opcode, cpu_state_t& state);

void _a0_ana_b(uint8_t * opcode, cpu_state_t& state);
void _a1_ana_c(uint8_t * opcode, cpu_state_t& state);
void _a2_ana_d(uint8_t * opcode, cpu_state_t& state);
void _a3_ana_e(uint8_t * opcode, cpu_state_t& state);
void _a4_ana_h(uint8_t * opcode, cpu_state_t& state);
void _a5_ana_l(uint8_t * opcode, cpu_state_t& state);
void _a6_ana_m(uint8_t * opcode, cpu_state_t& state);
void _a7_ana_a(uint8_t * opcode, cpu_state_t& state);

void _a8_xra_b(uint8_t * opcode, cpu_state_t& state);
void _a9_xra_c(uint8_t * opcode, cpu_state_t& state);
void _aa_xra_d(uint8_t * opcode, cpu_state_t& state);
void _ab_xra_e(uint8_t * opcode, cpu_state_t& state);
void _ac_xra_h(uint8_t * opcode, cpu_state_t& state);
void _ad_xra_l(uint8_t * opcode, cpu_state_t& state);
void _ae_xra_m(uint8_t * opcode, cpu_state_t& state);
void _af_xra_a(uint8_t * opcode, cpu_state_t& state);

void _b0_ora_b(uint8_t * opcode, cpu_state_t& state);
void _b1_ora_c(uint8_t * opcode, cpu_state_t& state);
void _b2_ora_d(uint8_t * opcode, cpu_state_t& state);
void _b3_ora_e(uint8_t * opcode, cpu_state_t& state);
void _b4_ora_h(uint8_t * opcode, cpu_state_t& state);
void _b5_ora_l(uint8_t * opcode, cpu_state_t& state);
void _b6_ora_m(uint8_t * opcode, cpu_state_t& state);
void _b7_ora_a(uint8_t * opcode, cpu_state_t& state);

void _b8_cmp_b(uint8_t * opcode, cpu_state_t& state);
void _b9_cmp_c(uint8_t * opcode, cpu_state_t& state);
void _ba_cmp_d(uint8_t * opcode, cpu_state_t& state);
void _bb_cmp_e(uint8_t * opcode, cpu_state_t& state);
void _bc_cmp_h(uint8_t * opcode, cpu_state_t& state);
void _bd_cmp_l(uint8_t * opcode, cpu_state_t& state);
void _be_cmp_m(uint8_t * opcode, cpu_state_t& state);
void _bf_cmp_a(uint8_t * opcode, cpu_state_t& state);

void _c6_adi_d8(uint8_t * opcode, cpu_state_t& state);
void _ce_aci_d8(uint8_t * opcode, cpu_state_t& state);
void _d6_sui_d8(uint8_t * opcode, cpu_state_t& state);
void _de_sbi_d8(uint8_t * opcode, cpu_state_t& state);
void _e6_ani_d8(uint8_t * opcode, cpu_state_t& state);
void _ee_xri_d8(uint8_t * opcode, cpu_state_t& state);
void _f6_ori_d8(uint8_t * opcode, cpu_state_t& state);
void _fe_cpi_d8(uint8_t * opcode, cpu_state_t& state);
// +++++++ END: 8 bits arithmetic/logical instructions

// +++++++ START: 16 bits arithmetic/logical instructions
void _03_inx_b_d16(uint8_t * opcode, cpu_state_t& state);
void _09_dad_b_d16(uint8_t * opcode, cpu_state_t& state);
void _0b_dcx_b_d16(uint8_t * opcode, cpu_state_t& state);
void _13_inx_d_d16(uint8_t * opcode, cpu_state_t& state);
void _19_dad_d_d16(uint8_t * opcode, cpu_state_t& state);
void _1b_dcx_d_d16(uint8_t * opcode, cpu_state_t& state);
void _23_inx_h_d16(uint8_t * opcode, cpu_state_t& state);
void _29_dad_h_d16(uint8_t * opcode, cpu_state_t& state);
void _2b_dcx_h_d16(uint8_t * opcode, cpu_state_t& state);
void _33_inx_sp_d16(uint8_t * opcode, cpu_state_t& state);
void _39_dad_sp_d16(uint8_t * opcode, cpu_state_t& state);
void _3b_dcx_sp_d16(uint8_t * opcode, cpu_state_t& state);
// +++++++ END: 16 bits arithmetic/logical instructions
