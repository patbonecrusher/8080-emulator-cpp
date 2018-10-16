An 8080 emulator is a computer

The computer runs a program on a cpu
The computer loads the program at a given memory address and starts the cpu
The cpu relies on the clock to execute instructions at the desire speed (it can go at 1x, 2x, 3x, 20x...)

The cpu uses an 8080 has a pc, sp, registers and a sets of flag
The cpu defines a list of 256 instructions possible
Each instruction has a cycle count, and an opsize (how many byte the instruction is in binary)
Each instruction is invoked with opcode information (data, next opcode address..), the current state (registers, flags) and returns the cycle count
The cpu keeps a current state of the registers, pc and sp and flags
The cpu uses the decoder to get the next instruction
The decoder relies on the state and the memory to decode the next instruction
The cpu then execute the instruction returned by the decoder
