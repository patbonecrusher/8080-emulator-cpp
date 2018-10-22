#pragma once

#include <cstddef>
#include <cstdint>

class IInputDevice {
public:
    virtual ~IInputDevice() {}; // destructor, use it to call destructor of the inherit classes
    virtual std::byte read(std::uint16_t port) = 0;
};

