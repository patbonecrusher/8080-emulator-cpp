#pragma once

#include <cstddef>
#include <cstdint>

class IOutputDevice {
public:
    virtual ~IInputDevice() {}; // destructor, use it to call destructor of the inherit classes
    virtual void read(std::uint16_t port, std::byte data) = 0;
};

