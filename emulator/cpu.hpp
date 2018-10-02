
#include "cpu_state.hpp"
#include "instruction.hpp"

class cpu {
    public:
        cpu();
        ~cpu();

        void load_instruction_set();

        // load binary data into memory
        void load_firmware_in_memory(uint8_t *mem_buffer, uint16_t address);

        // return a copy of the state.
        void step();

    private:

    public:
        uint8_t           *memory;
    private:
        cpu_state_t       cpu_state;
        instruction_map_t instructions;
};