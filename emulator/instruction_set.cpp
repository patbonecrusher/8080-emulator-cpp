#include "cpu_state.hpp"
#include "instruction_set.hpp"

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
extern void _f3_di(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.int_enable = false; }
extern void _fb_ei(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.int_enable = true; }

void _d3_out_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { 
//          this.writePort(this.nextByte(), this.a);
}
void _db_in_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
          // this.a = this.readPort(this.nextByte());

}

// +++++++ END: Misc/Control instructions

// +++++++ Jump/Call instructions
extern void _c0_rnz(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  if (!state.cc.z) {
    state.pc = (state.memory[state.sp+1]<<8 | state.memory[state.sp]); 
    state.sp += 2;
  }
}
extern void _c2_jnz_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  if (!state.cc.z) state.pc = OPCODE_TO16BIT(opcode); // else is no-op
}
extern void _c3_jmp_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { 
  state.pc = OPCODE_TO16BIT(opcode);
  if (state.pc == 0) {
    throw std::system_error(EFAULT, std::generic_category());
  }
}

extern void _c4_cnz_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // 	if NZ, CALL adr
  if (state.cc.z == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
extern void _c7_rst_0(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {exit(1);}
extern void _c8_rz(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if Z, RET
  if (state.cc.z == 1) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  } 
}
extern void _c9_ret(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  state.pc = state.memory[state.sp] | (state.memory[state.sp+1] << 8);
  state.sp += 2;
  if (state.pc == 0) {
    throw std::system_error(EFAULT, std::generic_category());
  }
}

extern void _ca_jz_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  if (state.cc.z) state.pc = OPCODE_TO16BIT(opcode);
}
// extern void _cb_jmp_a16_alt(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {

// }
extern void _cc_cz_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // 	if Z, CALL adr
  if (state.cc.z == 1) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}

extern void _cd_call_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
    if (5 ==  ((opcode[1] << 8) | opcode[0]))    
    {    
        if (state.c == 9)    
        {    
            uint16_t offset = (state.d<<8) | (state.e);    
            char *str = (char*) &state.memory[offset];  //skip the prefix bytes    
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
extern void _cf_rst_1(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // CALL $8
  uint16_t	ret = state.pc;
  state.write_mem(state.sp-1, (ret >> 8) & 0xff);
  state.write_mem(state.sp-2, (ret & 0xff));
	state.sp = state.sp - 2;
	state.pc = 0x0008;
}

extern void _d0_rnc(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if NCY, RET
  if (state.cc.cy == 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  } 
}

extern void _d2_jnc_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if N CY, PC<-adr
  if (!state.cc.cy) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _d4_cnc_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // 	if NCY, CALL adr
  if (state.cc.cy == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
extern void _d7_rst_2(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // CALL $10

}
extern void _d8_rc(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if CY, RET
  if (state.cc.cy != 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }  
}
// extern void _d9_ret_alt(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {exit(1);}
extern void _da_jc_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
    if (state.cc.cy) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _dc_cc_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if CY, CALL adr 
  // CAUSE THIS IS OUR RETURN ADDRESS
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
// extern void _dd_call_a16_alt(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {exit(1);}
extern void _df_rst_3(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint16_t	ret = state.pc;
  state.write_mem(state.sp-1, (ret >> 8) & 0xff);
  state.write_mem(state.sp-2, (ret & 0xff));
  state.sp = state.sp - 2;
  state.pc = 0x18;
}

extern void _e0_rpo(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // 	if PO, RET
  if (state.cc.p == 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }
}
extern void _e2_jpo_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  if (!state.cc.p) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _e4_cpo_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if PO, CALL adr
  if (state.cc.p == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}

extern void _e7_rst_4(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint16_t	ret = state.pc;
  state.write_mem(state.sp-1, (ret >> 8) & 0xff);
  state.write_mem(state.sp-2, (ret & 0xff));
  state.sp = state.sp - 2;
  state.pc = 0x20;
}
extern void _e8_rpe(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if PE, RET
  if (state.cc.p != 0) {
		state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
		state.sp += 2;
	}
}
extern void _e9_phcl(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  state.pc = (state.h << 8) | state.l;
}
extern void _ea_jpe_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
    if (state.cc.p) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _ec_cpe_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if PE, CALL adr
  if (state.cc.p != 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}

// extern void _ed_call_a16_alt(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {exit(1);}
extern void _ef_rst_5(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint16_t	ret = state.pc;
  state.write_mem(state.sp-1, (ret >> 8) & 0xff);
  state.write_mem(state.sp-2, (ret & 0xff));
  state.sp = state.sp - 2;
  state.pc = 0x28;
}

extern void _f0_rp(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if P, RET
  if (state.cc.s == 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }    
}
extern void _f2_jp_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  if (state.cc.p) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _f4_cp_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if P, PC <- adr
  if (state.cc.s == 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
extern void _f7_rst_6(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint16_t	ret = state.pc;
  state.write_mem(state.sp-1, (ret >> 8) & 0xff);
  state.write_mem(state.sp-2, (ret & 0xff));
  state.sp = state.sp - 2;
  state.pc = 0x30;
}
extern void _f8_rm(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if M, RET
  if (state.cc.s != 0) {
    state.pc = state.memory[state.sp] | (state.memory[state.sp+1]<<8);
    state.sp += 2;
  }    
}
extern void _fa_jm_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  if (state.cc.s) state.pc = OPCODE_TO16BIT(opcode);
}
extern void _fc_cm_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // if M, CALL adr
  if (state.cc.s != 0) {
    uint16_t ret = state.pc;
    state.write_mem(state.sp-1, (ret >> 8) & 0xff);
    state.write_mem(state.sp-2, (ret & 0xff));
    state.sp -= 2;
    state.pc = OPCODE_TO16BIT(opcode);
  }
}
// extern void _fd_call_a16_alt(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {exit(1);}
extern void _ff_rst_7(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint16_t	ret = state.pc;
  state.write_mem(state.sp-1, (ret >> 8) & 0xff);
  state.write_mem(state.sp-2, (ret & 0xff));
  state.sp = state.sp - 2;
  state.pc = 0x38;
}

// +++++++ END: Jump/Calls instructions

// +++++++ START: 8 bits load/store instructions

void _02_stax_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // (BC)<-A
  uint16_t bc = (state.b << 8) | state.c;
  state.write_mem(bc, state.a);
}
extern void _06_mvi_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // B<-byte2
  state.b = opcode[0];
}
void _0a_ldax_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A<-(BC)
  uint16_t bc = (state.b << 8) | state.c;
  state.a = state.memory[bc];
}
void _0e_mvi_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // C<-byte2
  state.c = opcode[0];
}

void _12_stax_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // (DE)<-A
  uint16_t de = (state.d << 8) | state.e;
  state.write_mem(de, state.a);
}
void _16_mvi_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // D<-byte2
  state.d = opcode[0];
}
void _1a_ldax_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A<-(DE)
  uint16_t offset=(state.d<<8) | state.e;
  state.a = state.memory[offset];
}

void _1e_mvi_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // E<-byte2
  state.e = opcode[0];
}

void _26_mvi_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = opcode[0]; }
void _2e_mvi_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = opcode[0]; }

void _32_sta_a16_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // (adr)<-A
  uint16_t addr = OPCODE_TO16BIT(opcode);
  state.write_mem(addr, state.a);
}
void _36_mvi_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint16_t addr = opcode[0];
  state.write_to_hl(opcode[0]);
}
void _3a_lda_a16_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint16_t addr = OPCODE_TO16BIT(opcode);
  state.a = state.memory[addr];
}
extern void _3e_mvi_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A<-byte2
  state.a = opcode[0];
}

void _40_mov_b_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.b; }
void _41_mov_b_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.c; }
void _42_mov_b_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.d; }
void _43_mov_b_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.e; }
void _44_mov_b_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.h; }
void _45_mov_b_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.l; }
void _46_mov_b_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.read_from_hl(); }
void _47_mov_b_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b = state.a; }

void _48_mov_c_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.b; }
void _49_mov_c_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.c; }
void _4a_mov_c_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.d; }
void _4b_mov_c_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.e; }
void _4c_mov_c_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.h; }
void _4d_mov_c_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.l; }
void _4e_mov_c_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.read_from_hl(); }
void _4f_mov_c_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c = state.a; }

void _50_mov_d_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.b; }
void _51_mov_d_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.c; }
void _52_mov_d_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.d; }
void _53_mov_d_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.e; }
void _54_mov_d_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.h; }
void _55_mov_d_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.l; }
void _56_mov_d_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.read_from_hl(); }
void _57_mov_d_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d = state.a; }

void _58_mov_e_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.b; }
void _59_mov_e_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.c; }
void _5a_mov_e_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.d; }
void _5b_mov_e_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.e; }
void _5c_mov_e_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.h; }
void _5d_mov_e_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.l; }
void _5e_mov_e_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.read_from_hl(); }
void _5f_mov_e_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e = state.a; }

void _60_mov_h_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.b; }
void _61_mov_h_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.c; }
void _62_mov_h_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.d; }
void _63_mov_h_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.e; }
void _64_mov_h_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.h; }
void _65_mov_h_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.l; }
void _66_mov_h_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.read_from_hl(); }
void _67_mov_h_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h = state.a; }

void _68_mov_l_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.b; }
void _69_mov_l_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.c; }
void _6a_mov_l_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.d; }
void _6b_mov_l_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.e; }
void _6c_mov_l_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.h; }
void _6d_mov_l_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.l; }
void _6e_mov_l_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.read_from_hl(); }
void _6f_mov_l_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l = state.a; }

void _70_mov_m_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.write_to_hl(state.b); }
void _71_mov_m_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.write_to_hl(state.c); }
void _72_mov_m_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.write_to_hl(state.d); }
void _73_mov_m_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.write_to_hl(state.e); }
void _74_mov_m_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.write_to_hl(state.h); }
void _75_mov_m_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.write_to_hl(state.l); }
void _77_mov_m_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.write_to_hl(state.a); }

void _78_mov_a_b_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.b; }
void _79_mov_a_c_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.c; }
void _7a_mov_a_d_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.d; }
void _7b_mov_a_e_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.e; }
void _7c_mov_a_h_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.h; }
void _7d_mov_a_l_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.l; }
void _7e_mov_a_m_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.read_from_hl(); }
void _7f_mov_a_a_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = state.a; }


// +++++++ END: 16 bits load/store instructions

// +++++++ START: 16 bits load/store instructions
extern void _01_lxi_b_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // B<-byte3,C<-byte2
  state.c = opcode[0];
  state.b = opcode[1];
}
extern void _11_lxi_d_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // D<-byte3,E<-byte2
  state.e = opcode[0];
  state.d = opcode[1];
}

extern void _21_lxi_h_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // H<-byte3,L<-byte2
  state.l = opcode[0];
  state.h = opcode[1];
}

extern void _22_shld_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // (adr)<-L;(adr+1)<-H
  uint16_t address = OPCODE_TO16BIT(opcode);
  state.write_mem(address, state.l);
  state.write_mem(address+1, state.h);
}

extern void _2a_lhld_a16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // L<-(adr);H<-(adr+1)
  uint16_t address = OPCODE_TO16BIT(opcode);
  state.l = state.memory[address];
  state.h = state.memory[address+1];
}

extern void _31_lxi_sp_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // SP.hi<-byte3,SP.lo<-byte2
  state.sp = OPCODE_TO16BIT(opcode);
}

void _c1_pop_b_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state)  { state.stack_pop(&state.b, &state.c); }
void _c5_push_b_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.stack_push(state.b, state.c); }
void _d1_pop_d_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state)  { state.stack_pop(&state.d, &state.e); }
void _d5_push_d_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.stack_push(state.d, state.e); }

void _e1_pop_h_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.stack_pop(&state.h, &state.l); }
void _e3_xhtl_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // L<->(SP);H<->(SP+1)
    uint8_t h = state.h;
    uint8_t l = state.l;
    state.l = state.memory[state.sp];
    state.h = state.memory[state.sp+1]; 
    state.write_mem(state.sp, l );
    state.write_mem(state.sp+1, h );
}

void _e5_push_h_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.stack_push(state.h, state.l); }
void _eb_xchg_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // H<->D;L<->E
  uint8_t h = state.h;
  uint8_t l = state.l;
  state.h = state.d;
  state.l = state.e;
  state.d = h;
  state.e = l;
}

void _f1_pop_psw_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.stack_pop(&state.a, (uint8_t*) &state.cc);}
void _f5_push_psw_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.stack_push(state.a, *(uint8_t*) &state.cc); }
void _f9_sphl_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.sp = state.l | (state.h << 8); }


// +++++++ END: 16 bits load/store instructions

// +++++++ START: 8 bits arithmetic/logical instructions

void _04_inr_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b += 1; state.flagZSP(state.b); }
void _0c_inr_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c += 1; state.flagZSP(state.c); }
void _14_inr_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d += 1; state.flagZSP(state.d); }
void _1c_inr_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e += 1; state.flagZSP(state.e); }
void _24_inr_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h += 1; state.flagZSP(state.h); }
void _2c_inr_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l += 1; state.flagZSP(state.l); }
void _34_inr_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { 
  uint8_t res = state.read_from_hl() + 1;
  state.flagZSP(res);
  state.write_to_hl(res);
}
void _3c_inr_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a += 1; state.flagZSP(state.a); }

void _05_dcr_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.b -= 1; state.flagZSP(state.b); }
void _0d_dcr_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.c -= 1; state.flagZSP(state.c); }
void _15_dcr_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.d -= 1; state.flagZSP(state.d); }
void _1d_dcr_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.e -= 1; state.flagZSP(state.e); }
void _25_dcr_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.h -= 1; state.flagZSP(state.h); }
void _2d_dcr_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.l -= 1; state.flagZSP(state.l); }
void _35_dcr_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { 
  uint8_t res = state.read_from_hl() - 1;
  state.flagZSP(res);
  state.write_to_hl(res);
}
void _3d_dcr_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a -= 1; state.flagZSP(state.a); }

void _07_rlc(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint8_t x = state.a;
  state.a = ((x & 0x80) >> 7) | (x << 1);
  state.cc.cy = (0x80 == (x&0x80));
}
void _0f_rrc(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint8_t x = state.a;
  state.a = ((x & 1) << 7) | (x >> 1);
  state.cc.cy = (1 == (x&1));
}
void _17_ral(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint8_t x = state.a;
  state.a = state.cc.cy  | (x << 1);
  state.cc.cy = (0x80 == (x&0x80));
}
void _1f_rar(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  uint8_t x = state.a;
  state.a = (state.cc.cy << 7) | (x >> 1);
  state.cc.cy = (1 == (x&1));
}
void _27_daa(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // decimal adjust accumulator
  uint16_t a = state.a;
  if ((a & 0x0f) > 0x09 || state.cc.ac == 1) {
    if (((a & 0x0f) + 0x06) & 0xF0) {
      state.cc.ac = 1;
    } else {
      state.cc.ac = 0;
    }
    a += 0x06;
    if (a & 0xFF00) {
      state.cc.cy = 1;
    }
  }
  if (((a & 0xF0) > 0x90) || state.cc.cy == 1) {
    a += 0x60;
    if (a & 0xFF00) {
      state.cc.cy = 1;
    }
  }
  state.flagZSP(a);
  state.a = a;

                // temp16 = reg8[A];
                // if (((temp16 & 0x0F) > 0x09) || test_AC()) {
                //     if (((temp16 & 0x0F) + 0x06) & 0xF0) set_AC(); else clear_AC();
                //     temp16 += 0x06;
                //     if (temp16 & 0xFF00) set_C(); //can also cause carry to be set during addition to the low nibble
                // }
                // if (((temp16 & 0xF0) > 0x90) || test_C()) {
                //     temp16 += 0x60;
                //     if (temp16 & 0xFF00) set_C(); //doesn't clear it if this clause is false
                // }
                // calc_SZP((uint8_t)temp16);
                // reg8[A] = (uint8_t)temp16;

}
void _2f_cma(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.a = ~state.a; }
void _37_stc(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.cc.cy = 1; }
void _3f_cmc(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.cc.cy = 0; } // state->cc.cy = !state->cc.cy;}

void _80_add_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.b); }
void _81_add_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.c); }
void _82_add_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.d); }
void _83_add_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.e); }
void _84_add_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.h); }
void _85_add_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.l); }
void _86_add_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.read_from_hl()); }
void _87_add_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_add_ops(state.a); }

void _88_adc_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.b); }
void _89_adc_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.c); }
void _8a_adc_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.d); }
void _8b_adc_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.e); }
void _8c_adc_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.h); }
void _8d_adc_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.l); }
void _8e_adc_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.read_from_hl()); }
void _8f_adc_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_adc_ops(state.a); }

void _90_sub_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.b); }
void _91_sub_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.c); }
void _92_sub_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.d); }
void _93_sub_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.e); }
void _94_sub_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.h); }
void _95_sub_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.l); }
void _96_sub_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.read_from_hl()); }
void _97_sub_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sub_ops(state.a); }

void _98_sbb_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.b); }
void _99_sbb_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.c); }
void _9a_sbb_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.d); }
void _9b_sbb_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.e); }
void _9c_sbb_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.h); }
void _9d_sbb_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.l); }
void _9e_sbb_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.read_from_hl()); }
void _9f_sbb_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_sbb_ops(state.a); }

void _a0_ana_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.b); }
void _a1_ana_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.c); }
void _a2_ana_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.d); }
void _a3_ana_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.e); }
void _a4_ana_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.h); }
void _a5_ana_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.l); }
void _a6_ana_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.read_from_hl()); }
void _a7_ana_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ana_ops(state.a); }

void _a8_xra_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.b); }
void _a9_xra_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.c); }
void _aa_xra_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.d); }
void _ab_xra_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.e); }
void _ac_xra_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.h); }
void _ad_xra_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.l); }
void _ae_xra_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.read_from_hl()); }
void _af_xra_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_xra_ops(state.a); }

void _b0_ora_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.b); }
void _b1_ora_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.c); }
void _b2_ora_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.d); }
void _b3_ora_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.e); }
void _b4_ora_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.h); }
void _b5_ora_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.l); }
void _b6_ora_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.read_from_hl()); }
void _b7_ora_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_ora_ops(state.a); }

void _b8_cmp_b(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.b); }
void _b9_cmp_c(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.c); }
void _ba_cmp_d(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.d); }
void _bb_cmp_e(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.e); }
void _bc_cmp_h(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.h); }
void _bd_cmp_l(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.l); }
void _be_cmp_m(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.read_from_hl()); }
void _bf_cmp_a(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) { state.do_cmp_ops(state.a); }

void _c6_adi_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A<-A+data
  uint16_t x = (uint16_t) state.a + (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _ce_aci_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // 	A <- A + data + CY
  uint16_t x = (uint16_t) state.a + (uint16_t) opcode[0] + state.cc.cy;
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _d6_sui_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A <- A - data
  uint16_t x = (uint16_t) state.a - (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _de_sbi_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A <- A - data - CY
  uint16_t x = (uint16_t) state.a - (uint16_t) opcode[0] - state.cc.cy;
  state.flagZSP(x & 0xff);
	state.cc.cy = (x > 0xff);
	state.a = x & 0xff;
}

void _e6_ani_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A<-A&data
  uint16_t x = (uint16_t) state.a & (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = 0;
	state.a = x & 0xff;

  // state.logic_flags_a(state.a & opcode[0]);
}

void _ee_xri_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A<-A^data
  uint16_t x = (uint16_t) state.a ^ (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = 0;
	state.a = x & 0xff;
}

void _f6_ori_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A<-A|data
  uint16_t x = (uint16_t) state.a | (uint16_t) opcode[0];
  state.flagZSP(x & 0xff);
	state.cc.cy = 0;
	state.a = x & 0xff;
}

void _fe_cpi_d8(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // A-data
  uint8_t x = state.a - opcode[0];
  state.flagZSP(x);
  state.cc.cy = (state.a < opcode[0]);
}

// +++++++ END: 8 bits arithmetic/logical instructions

// +++++++ START: 16 bits arithmetic/logical instructions
void _03_inx_b_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // BC<-BC+1
  state.c++;
  if (0 == state.c) state.b++;
}
void _09_dad_b_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // HL=HL+BC
  uint32_t hl = (state.h << 8) | state.l;
  uint32_t bc = (state.b << 8) | state.c;
  uint32_t res = hl + bc;
  state.h = (res & 0xff00) >> 8;
  state.l = (res & 0xff);
  state.cc.cy = ((res & 0xffff0000) != 0);
}
void _0b_dcx_b_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // BC=BC-1
  state.c--;
  if (0xff == state.c) state.b--;
}
void _13_inx_d_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // DE<-DE+1
  state.e++;
  if (0 == state.e) state.d++;
}
void _19_dad_d_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // HL=HL+DE
  uint32_t hl = (state.h << 8) | state.l;
  uint32_t de = (state.d << 8) | state.e;
  uint32_t res = hl + de;
  state.h = (res & 0xff00) >> 8;
  state.l = (res & 0xff);
  state.cc.cy = ((res & 0xffff0000) != 0);
}
void _1b_dcx_d_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // E=E-1
  state.e--;
  if (0xff == state.e) state.d--;
}

void _23_inx_h_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // HL<-HL+1
  state.l++;
  if (0 == state.l) state.h++;
}
void _29_dad_h_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // HL=HL+HL
  uint32_t hl = (state.h << 8) | state.l;
  uint32_t res = hl + hl;
  state.h = (res & 0xff00) >> 8;
  state.l = (res & 0xff);
  state.cc.cy = ((res & 0xffff0000) != 0);
}
void _2b_dcx_h_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // H=H-1
  state.l--;
  if (0xff == state.l) state.h--;
}
void _33_inx_sp_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  state.sp++;
}
void _39_dad_sp_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  // HL=HL+SP
  uint32_t hl = (state.h << 8) | state.l;
  uint32_t res = hl + state.sp;
  state.h = (res & 0xff00) >> 8;
  state.l = (res & 0xff);
  state.cc.cy = ((res & 0xffff0000) != 0);
}
void _3b_dcx_sp_d16(uint8_t * opcode,  op_info_t& op_info,  cpu_state_t& state) {
  state.sp--;
}
// +++++++ END: 16 bits arithmetic/logical instructions