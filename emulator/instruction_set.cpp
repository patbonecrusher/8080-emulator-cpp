#include "cpu_state.hpp"

/*
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
|    | x0                     | x1                         | x2                       | x3                      | x4                         | x5                       | x6                       | x7                     | x8                     | x9                     | xA                       | xB                       | xC                         | xD                        | xE                      | xF                     |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 0x | NOP 1  4 - - - - -     | LXI B,d16 3  10 - - - - -  | STAX B 1  7 - - - - -    | INX B 1  5 - - - - -    | INR B 1  5 S Z A P -       | DCR B 1  5 S Z A P -     | MVI B,d8 2  7 - - - - -  | RLC 1  4 - - - - C     | *NOP 1  4 - - - - -    | DAD B 1  10 - - - - C  | LDAX B 1  7 - - - - -    | DCX B 1  5 - - - - -     | INR C 1  5 S Z A P -       | DCR C 1  5 S Z A P -      | MVI C,d8 2  7 - - - - - | RRC 1  4 - - - - C     |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 1x | *NOP 1  4 - - - - -    | LXI D,d16 3  10 - - - - -  | STAX D 1  7 - - - - -    | INX D 1  5 - - - - -    | INR D 1  5 S Z A P -       | DCR D 1  5 S Z A P -     | MVI D,d8 2  7 - - - - -  | RAL 1  4 - - - - C     | *NOP 1  4 - - - - -    | DAD D 1  10 - - - - C  | LDAX D 1  7 - - - - -    | DCX D 1  5 - - - - -     | INR E 1  5 S Z A P -       | DCR E 1  5 S Z A P -      | MVI E,d8 2  7 - - - - - | RAR 1  4 - - - - C     |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 2x | *NOP 1  4 - - - - -    | LXI H,d16 3  10 - - - - -  | SHLD a16 3  16 - - - - - | INX H 1  5 - - - - -    | INR H 1  5 S Z A P -       | DCR H 1  5 S Z A P -     | MVI H,d8 2  7 - - - - -  | DAA 1  4 S Z A P C     | *NOP 1  4 - - - - -    | DAD H 1  10 - - - - C  | LHLD a16 3  16 - - - - - | DCX H 1  5 - - - - -     | INR L 1  5 S Z A P -       | DCR L 1  5 S Z A P -      | MVI L,d8 2  7 - - - - - | CMA 1  4 - - - - -     |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 3x | *NOP 1  4 - - - - -    | LXI SP,d16 3  10 - - - - - | STA a16 3  13 - - - - -  | INX SP 1  5 - - - - -   | INR M 1  10 S Z A P -      | DCR M 1  10 S Z A P -    | MVI M,d8 2  10 - - - - - | STC 1  4 - - - - C     | *NOP 1  4 - - - - -    | DAD SP 1  10 - - - - C | LDA a16 3  13 - - - - -  | DCX SP 1  5 - - - - -    | INR A 1  5 S Z A P -       | DCR A 1  5 S Z A P -      | MVI A,d8 2  7 - - - - - | CMC 1  4 - - - - C     |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 4x | MOV B,B 1  5 - - - - - | MOV B,C 1  5 - - - - -     | MOV B,D 1  5 - - - - -   | MOV B,E 1  5 - - - - -  | MOV B,H 1  5 - - - - -     | MOV B,L 1  5 - - - - -   | MOV B,M 1  7 - - - - -   | MOV B,A 1  5 - - - - - | MOV C,B 1  5 - - - - - | MOV C,C 1  5 - - - - - | MOV C,D 1  5 - - - - -   | MOV C,E 1  5 - - - - -   | MOV C,H 1  5 - - - - -     | MOV C,L 1  5 - - - - -    | MOV C,M 1  7 - - - - -  | MOV C,A 1  5 - - - - - |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 5x | MOV D,B 1  5 - - - - - | MOV D,C 1  5 - - - - -     | MOV D,D 1  5 - - - - -   | MOV D,E 1  5 - - - - -  | MOV D,H 1  5 - - - - -     | MOV D,L 1  5 - - - - -   | MOV D,M 1  7 - - - - -   | MOV D,A 1  5 - - - - - | MOV E,B 1  5 - - - - - | MOV E,C 1  5 - - - - - | MOV E,D 1  5 - - - - -   | MOV E,E 1  5 - - - - -   | MOV E,H 1  5 - - - - -     | MOV E,L 1  5 - - - - -    | MOV E,M 1  7 - - - - -  | MOV E,A 1  5 - - - - - |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 6x | MOV H,B 1  5 - - - - - | MOV H,C 1  5 - - - - -     | MOV H,D 1  5 - - - - -   | MOV H,E 1  5 - - - - -  | MOV H,H 1  5 - - - - -     | MOV H,L 1  5 - - - - -   | MOV H,M 1  7 - - - - -   | MOV H,A 1  5 - - - - - | MOV L,B 1  5 - - - - - | MOV L,C 1  5 - - - - - | MOV L,D 1  5 - - - - -   | MOV L,E 1  5 - - - - -   | MOV L,H 1  5 - - - - -     | MOV L,L 1  5 - - - - -    | MOV L,M 1  7 - - - - -  | MOV L,A 1  5 - - - - - |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 7x | MOV M,B 1  7 - - - - - | MOV M,C 1  7 - - - - -     | MOV M,D 1  7 - - - - -   | MOV M,E 1  7 - - - - -  | MOV M,H 1  7 - - - - -     | MOV M,L 1  7 - - - - -   | HLT 1  7 - - - - -       | MOV M,A 1  7 - - - - - | MOV A,B 1  5 - - - - - | MOV A,C 1  5 - - - - - | MOV A,D 1  5 - - - - -   | MOV A,E 1  5 - - - - -   | MOV A,H 1  5 - - - - -     | MOV A,L 1  5 - - - - -    | MOV A,M 1  7 - - - - -  | MOV A,A 1  5 - - - - - |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 8x | ADD B 1  4 S Z A P C   | ADD C 1  4 S Z A P C       | ADD D 1  4 S Z A P C     | ADD E 1  4 S Z A P C    | ADD H 1  4 S Z A P C       | ADD L 1  4 S Z A P C     | ADD M 1  7 S Z A P C     | ADD A 1  4 S Z A P C   | ADC B 1  4 S Z A P C   | ADC C 1  4 S Z A P C   | ADC D 1  4 S Z A P C     | ADC E 1  4 S Z A P C     | ADC H 1  4 S Z A P C       | ADC L 1  4 S Z A P C      | ADC M 1  7 S Z A P C    | ADC A 1  4 S Z A P C   |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| 9x | SUB B 1  4 S Z A P C   | SUB C 1  4 S Z A P C       | SUB D 1  4 S Z A P C     | SUB E 1  4 S Z A P C    | SUB H 1  4 S Z A P C       | SUB L 1  4 S Z A P C     | SUB M 1  7 S Z A P C     | SUB A 1  4 S Z A P C   | SBB B 1  4 S Z A P C   | SBB C 1  4 S Z A P C   | SBB D 1  4 S Z A P C     | SBB E 1  4 S Z A P C     | SBB H 1  4 S Z A P C       | SBB L 1  4 S Z A P C      | SBB M 1  7 S Z A P C    | SBB A 1  4 S Z A P C   |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| Ax | ANA B 1  4 S Z A P C   | ANA C 1  4 S Z A P C       | ANA D 1  4 S Z A P C     | ANA E 1  4 S Z A P C    | ANA H 1  4 S Z A P C       | ANA L 1  4 S Z A P C     | ANA M 1  7 S Z A P C     | ANA A 1  4 S Z A P C   | XRA B 1  4 S Z A P C   | XRA C 1  4 S Z A P C   | XRA D 1  4 S Z A P C     | XRA E 1  4 S Z A P C     | XRA H 1  4 S Z A P C       | XRA L 1  4 S Z A P C      | XRA M 1  7 S Z A P C    | XRA A 1  4 S Z A P C   |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| Bx | ORA B 1  4 S Z A P C   | ORA C 1  4 S Z A P C       | ORA D 1  4 S Z A P C     | ORA E 1  4 S Z A P C    | ORA H 1  4 S Z A P C       | ORA L 1  4 S Z A P C     | ORA M 1  7 S Z A P C     | ORA A 1  4 S Z A P C   | CMP B 1  4 S Z A P C   | CMP C 1  4 S Z A P C   | CMP D 1  4 S Z A P C     | CMP E 1  4 S Z A P C     | CMP H 1  4 S Z A P C       | CMP L 1  4 S Z A P C      | CMP M 1  7 S Z A P C    | CMP A 1  4 S Z A P C   |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| Cx | RNZ 1  11/5 - - - - -  | POP B 1  10 - - - - -      | JNZ a16 3  10 - - - - -  | JMP a16 3  10 - - - - - | CNZ a16 3  17/11 - - - - - | PUSH B 1  11 - - - - -   | ADI d8 2  7 S Z A P C    | RST 0 1  11 - - - - -  | RZ 1  11/5 - - - - -   | RET 1  10 - - - - -    | JZ a16 3  10 - - - - -   | *JMP a16 3  10 - - - - - | CZ a16 3  17/11 - - - - -  | CALL a16 3  17 - - - - -  | ACI d8 2  7 S Z A P C   | RST 1 1  11 - - - - -  |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| Dx | RNC 1  11/5 - - - - -  | POP D 1  10 - - - - -      | JNC a16 3  10 - - - - -  | OUT d8 2  10 - - - - -  | CNC a16 3  17/11 - - - - - | PUSH D 1  11 - - - - -   | SUI d8 2  7 S Z A P C    | RST 2 1  11 - - - - -  | RC 1  11/5 - - - - -   | *RET 1  10 - - - - -   | JC a16 3  10 - - - - -   | IN d8 2  10 - - - - -    | CC a16 3  17/11 - - - - -  | *CALL a16 3  17 - - - - - | SBI d8 2  7 S Z A P C   | RST 3 1  11 - - - - -  |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| Ex | RPO 1  11/5 - - - - -  | POP H 1  10 - - - - -      | JPO a16 3  10 - - - - -  | XTHL 1  18 - - - - -    | CPO a16 3  17/11 - - - - - | PUSH H 1  11 - - - - -   | ANI d8 2  7 S Z A P C    | RST 4 1  11 - - - - -  | RPE 1  11/5 - - - - -  | PCHL 1  5 - - - - -    | JPE a16 3  10 - - - - -  | XCHG 1  5 - - - - -      | CPE a16 3  17/11 - - - - - | *CALL a16 3  17 - - - - - | XRI d8 2  7 S Z A P C   | RST 5 1  11 - - - - -  |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
| Fx | RP 1  11/5 - - - - -   | POP PSW 1  10 S Z A P C    | JP a16 3  10 - - - - -   | DI 1  4 - - - - -       | CP a16 3  17/11 - - - - -  | PUSH PSW 1  11 - - - - - | ORI d8 2  7 S Z A P C    | RST 6 1  11 - - - - -  | RM 1  11/5 - - - - -   | SPHL 1  5 - - - - -    | JM a16 3  10 - - - - -   | EI 1  4 - - - - -        | CM a16 3  17/11 - - - - -  | *CALL a16 3  17 - - - - - | CPI d8 2  7 S Z A P C   | RST 7 1  11 - - - - -  |
+----+------------------------+----------------------------+--------------------------+-------------------------+----------------------------+--------------------------+--------------------------+------------------------+------------------------+------------------------+--------------------------+--------------------------+----------------------------+---------------------------+-------------------------+------------------------+
*/

#define OPCODE_TO16BIT(opcode) (opcode[1]<<8) | opcode[0]

// +++++++ START: Misc/Control instructions
// NOP && HALT && OUT && IN && DI && EI Are no-op
extern void _f3_di(uint8_t * opcode, cpu_state_t& state) { state.int_enable = false; }
extern void _fb_ei(uint8_t * opcode, cpu_state_t& state) { state.int_enable = true; }
// +++++++ END: Misc/Control instructions

// +++++++ Jump/Call instructions
extern void _c0_rnz(uint8_t * opcode, cpu_state_t& state) {
  if (!state.cc.z) {
    state.pc = (state.memory[state.sp+1]<<8 | state.memory[state.sp]); 
    state.sp += 2;
  }
}
extern void _c2_jnz_a16(uint8_t * opcode, cpu_state_t& state) {
  if (!state.cc.z) state.pc = OPCODE_TO16BIT(opcode); // else is no-op
}
extern void _c3_jmp_a16(uint8_t * opcode, cpu_state_t& state) { 
  state.pc = OPCODE_TO16BIT(opcode);
}
extern void _c4_cnz_a16(uint8_t * opcode, cpu_state_t& state) {
  // 	if NZ, CALL adr
  if (state.cc.z == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
extern void _c7_rst_0(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _c8_rz(uint8_t * opcode, cpu_state_t& state) {
  // if Z, RET
  if (state.cc.z == 1) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  } 
}
extern void _c9_ret(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _ca_jz_a16(uint8_t * opcode, cpu_state_t& state) {
  if (state.cc.z) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _cb_jmp_a16_alt(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _cc_cz_a16(uint8_t * opcode, cpu_state_t& state) {
  // 	if Z, CALL adr
  if (state.cc.z == 1) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}

extern void _cd_call_a16(uint8_t * opcode, cpu_state_t& state) {
    if (5 ==  ((opcode[1] << 8) | opcode[0]))    
    {    
        if (state.c == 9)    
        {    
            uint16_t offset = (state.d<<8) | (state.e);    
            char *str = (char*) &state.memory[offset+3];  //skip the prefix bytes    
            while (*str != '$')    
                printf("%c", *str++);    
            printf("\n");    
        }    
        else if (state.c == 2)    
        {    
            //saw this in the inspected code, never saw it called    
            printf ("print char routine called\n");    
        }    
    }    
    else if (0 ==  ((opcode[1] << 8) | opcode[0]))    
    {    
        exit(0);    
    } else {
      // (SP-1)<-PC.hi;
      // (SP-2)<-PC.lo;
      // SP<-SP+2;
      // PC=adr
      uint16_t ret = state.pc;
      state.write_mem(state.sp-1, (state.pc >> 8) & 0xff);
      state.write_mem(state.sp-2, (state.pc & 0xff));
      state.sp = state.sp-2;
      state.pc = OPCODE_TO16BIT(opcode);
    }
}
extern void _cf_rst_1(uint8_t * opcode, cpu_state_t& state) {exit(1);}

extern void _d0_rnc(uint8_t * opcode, cpu_state_t& state) {
  // if NCY, RET
  if (state.cc.cy == 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  } 
}

extern void _d2_jnc_a16(uint8_t * opcode, cpu_state_t& state) {
  // if N CY, PC<-adr
  if (!state.cc.cy) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _d4_cnc_a16(uint8_t * opcode, cpu_state_t& state) {
  // 	if NCY, CALL adr
  if (state.cc.cy == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
extern void _d7_rst_2(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _d8_rc(uint8_t * opcode, cpu_state_t& state) {
  // if CY, RET
  if (state.cc.cy != 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }  
}
extern void _d9_ret_alt(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _da_jc_a16(uint8_t * opcode, cpu_state_t& state) {
    if (state.cc.cy) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _dc_cc_a16(uint8_t * opcode, cpu_state_t& state) {
  // if CY, CALL adr 
  // TODO: I DON'T UNDERSTAND THIS HERE
  // WHY ARE WE OPERATING ON THE NEXT INSTRUCTION ADDRESS.
  if (state.cc.cy) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }

}
extern void _dd_call_a16_alt(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _df_rst_3(uint8_t * opcode, cpu_state_t& state) {exit(1);}

extern void _e0_rpo(uint8_t * opcode, cpu_state_t& state) {
  // 	if PO, RET
  if (state.cc.p == 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }
}
extern void _e2_jpo_a16(uint8_t * opcode, cpu_state_t& state) {
  if (!state.cc.p) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _e4_cpo_a16(uint8_t * opcode, cpu_state_t& state) {
  // if PO, CALL adr
  if (state.cc.p == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}

extern void _e7_rst_4(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _e8_rpe(uint8_t * opcode, cpu_state_t& state) {
  // if PE, RET
  if (state.cc.p != 0) {
		state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
		state.sp += 2;
	}
}
extern void _e9_phcl(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _ea_jpe_a16(uint8_t * opcode, cpu_state_t& state) {
    if (state.cc.p) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _ec_cpe_a16(uint8_t * opcode, cpu_state_t& state) {
  // if PE, CALL adr
  if (state.cc.p != 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}

extern void _ed_call_a16_alt(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _ef_rst_5(uint8_t * opcode, cpu_state_t& state) {exit(1);}

extern void _f0_rp(uint8_t * opcode, cpu_state_t& state) {
  // if P, RET
  if (state.cc.s == 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }    
}
extern void _f2_jp_a16(uint8_t * opcode, cpu_state_t& state) {
  if (state.cc.p) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _f4_cp_a16(uint8_t * opcode, cpu_state_t& state) {
  // if P, PC <- adr
  if (state.cc.s == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
extern void _f7_rst_6(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _f8_rm(uint8_t * opcode, cpu_state_t& state) {
  // if M, RET
  if (state.cc.s != 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }    
}
extern void _fa_jm_a16(uint8_t * opcode, cpu_state_t& state) {
  if (state.cc.s) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _fc_cm_a16(uint8_t * opcode, cpu_state_t& state) {
  // if M, CALL adr
  if (state.cc.s != 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
extern void _fd_call_a16_alt(uint8_t * opcode, cpu_state_t& state) {exit(1);}
extern void _ff_rst_7(uint8_t * opcode, cpu_state_t& state) {exit(1);}

// +++++++ END: Jump/Calls instructions

// +++++++ START: 8 bits load/store instructions
extern void _06_mvi_b_d8(uint8_t * opcode, cpu_state_t& state) {
  // B<-byte2
  state.b = opcode[0];
}

void _1a_ldax_d_d8(uint8_t * opcode, cpu_state_t& state) {
  // A<-(DE)
  uint16_t offset=(state.d<<8) | state.e;
  state.a = state.memory[offset];
}

extern void _23_inx_h_d8(uint8_t * opcode, cpu_state_t& state) {
  // HL<-HL+1
  state.l++;
  if (0 == state.l) state.h++;
}

void _77_mov_m_a_d8(int8_t * opcode, cpu_state_t& state) {
  // (HL)<-C 
  uint16_t offset=(state.h<<8) | state.l;
  state.memory[offset] = state.a;
}

// +++++++ END: 16 bits load/store instructions

// +++++++ START: 16 bits load/store instructions
extern void _11_lxi_d_d16(uint8_t * opcode, cpu_state_t& state) {
  // D<-byte3,E<-byte2
  state.e = opcode[0];
  state.d = opcode[1];
}

extern void _21_lxi_h_d16(uint8_t * opcode, cpu_state_t& state) {
  // H<-byte3,L<-byte2
  state.l = opcode[0];
  state.h = opcode[1];
}

extern void _31_lxi_sp_d16(uint8_t * opcode, cpu_state_t& state) {
  // SP.hi<-byte3,SP.lo<-byte2
  state.sp = OPCODE_TO16BIT(opcode);
}

// +++++++ END: 16 bits load/store instructions

// +++++++ START: 8 bits arithmetic/logical instructions
void _05_dcr_b(uint8_t * opcode, cpu_state_t& state) {
  // B<-B-1
  state.b -= 1;
  state.flagZSP(state.b);
}

void _c6_adi_d8(uint8_t * opcode, cpu_state_t& state) {
  // A<-A+data
  uint16_t x = (uint16_t) state.a + (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _ce_aci_d8(uint8_t * opcode, cpu_state_t& state) {
  // 	A <- A + data + CY
  uint16_t x = (uint16_t) state.a + (uint16_t) opcode[0] + state.cc.cy;
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _d6_sui_d8(uint8_t * opcode, cpu_state_t& state) {
  // A <- A - data
  uint16_t x = (uint16_t) state.a - (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _de_sbi_d8(uint8_t * opcode, cpu_state_t& state) {
  // A <- A - data - CY
  uint16_t x = (uint16_t) state.a - (uint16_t) opcode[0] - state.cc.cy;
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _e6_ani_d8(uint8_t * opcode, cpu_state_t& state) {
  // A<-A&data
  state.logic_flags_a(state.a & opcode[0]);
}

void _ee_xri_d8(uint8_t * opcode, cpu_state_t& state) {
  // A<-A^data
  uint16_t x = (uint16_t) state.a ^ (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = 0;
	state.a = x & 0xff;
}

void _f6_ori_d8(uint8_t * opcode, cpu_state_t& state) {
  // A<-A|data
  uint16_t x = (uint16_t) state.a | (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = 0;
	state.a = x & 0xff;}

void _fe_cpi_d8(uint8_t * opcode, cpu_state_t& state) {
  // A-data
  uint8_t x = state.a - opcode[0];
  state.flagZSP(x);
  state.cc.cy = (state.a < opcode[0]);
}

// +++++++ END: 8 bits arithmetic/logical instructions

// +++++++ START: 16 bits arithmetic/logical instructions
void _13_inx_d_d16(uint8_t * opcode, cpu_state_t& state) {
  // DE<-DE+1
  state.d++;
  if (0 == state.d) state.e++;
}

// +++++++ END: 16 bits arithmetic/logical instructions