# Instruction
* mnemonic (its name)
* length (in bytes)
* duration (in cyles) (sometime min/max)
* flags
* code

## Instruction types
* misc/control instruction
* jump/calls
* 8 bits load/store/move
* 16 bits load/store/move
* 8 bits arithmetic/logical
* 16 bits arithmetics/logical

2MHZ = 2,000,000 clock cycles per second



Flags
The processor maintains internal flag bits (a status register), which indicate the results of arithmetic and logical instructions. The flags are:

Sign (S), set if the result is negative.
Zero (Z), set if the result is zero.
Parity (P), set if the number of 1 bits in the result is even.
Carry (C), set if the last addition operation resulted in a carry or if the last subtraction operation required a borrow
Auxiliary carry (AC or H), used for binary-coded decimal arithmetic (BCD).

The carry bit can be set or complemented by specific instructions. Conditional-branch instructions test the various flag status bits. The flags can be copied as a group to the accumulator. The A accumulator and the flags together are called the PSW register, or program status word.`


The CPU step method executes the next instruction and return the number of cycles spent
Allowing the CPU to wait some.

There is a way to figure out the clock cycle of a computer and from that, adjust the game speed.  We can run the cpu in slow mode using thread affinity, which differs from os.

We know the CPU is 2MGZ, 2M clock cycles per second
We know how many clock cycles were interpreted
