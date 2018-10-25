#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

typedef std::vector<std::byte>    byte_arr;
typedef byte_arr::iterator        byte_arr_it;
typedef byte_arr::const_iterator  byte_arr_const_it;

struct program {

  static byte_arr read_file_at_path(std::string const& path);

  std::uint16_t start_address;
  byte_arr      program_bin;
};