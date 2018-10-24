#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

typedef std::vector<std::byte>    byte_arr;
typedef byte_arr::iterator        byte_arr_it;
typedef byte_arr::const_iterator  byte_arr_const_it;

class program {

  public:
    program();
    ~program();

    program(const program&) = delete;
    program(program&&) = default;
    program& operator =(const program&) = delete;
    program& operator =(program&&) = delete;

  protected: 
    std::uint16_t start_address;
    byte_arr      program_bin;
};