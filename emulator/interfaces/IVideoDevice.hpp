#pragma once

#include <cstddef>
#include <cstdint>

class IVideoDevice {
public:
    virtual ~IVideoDevice() {}; // destructor, use it to call destructor of the inherit classes
    virtual void vblank() = 0;
};

