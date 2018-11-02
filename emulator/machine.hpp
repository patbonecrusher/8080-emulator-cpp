#pragma once

/*
machine
  -> bus
        -> provides access to memory
        -> interrupts
        -> io events
        -> video events
  -> ios
     -> interracts with the bus
     -> generate interrupts
  -> memory (this is app specific depends how the program is written)
     -> listen/interract with the bus
     -> contains program space, video ram space, system space
     -> capacity
  -> video controller
     -> interracts with the bus
  -> cpu
     -> throttle
     -> registers
        -> A, F, B, C, D, E, H, L
     -> status registers (flip flop)
        -> C, Reset, P, A, Z, S
     -> alu
        -> arithmetic operations
        -> handle carries
     -> instructions
        -> decode / executes instructions
        -> access cpu
        -> read/updates register
        -> read/updates status register
        -> access the alu
     -> interrupts
        -> irq
        -> vector
     -> pc
     -> sp
     -> bus

*/

class Machine {
    public:

    private:

};