#pragma once

#include <ctype>

using namespace std;

class MemoryController {
public:
    explicit MemoryController();
    ~MemoryController();

    MemoryController(const MemoryController&) = delete;
    MemoryController(MemoryController&&) = default;
    MemoryController& operator =(const MemoryController&) = delete;
    MemoryController& operator =(MemoryController&&) = delete;

// protected:
    // bus
    uint16_t capacity;
    uint8_t * memory;
    uint16_t program_start;
    uint16_t program_end;

};