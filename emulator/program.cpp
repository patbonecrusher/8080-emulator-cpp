// #include <fstream>
// #include <string>
// #include <vector>
// #include <iostream>
// #include <iomanip>
// #include <cstring>

// #include "program.hpp"

// // byte_arr Program::read_file_at_path(std::string const& filepath) {
// //   std::ifstream ifs(filepath, std::ios::binary|std::ios::ate);
// //   if (!ifs) throw std::runtime_error(filepath + ": " + std::strerror(errno));

// //   auto  end = ifs.tellg();
// //   ifs.seekg(0, std::ios::beg);

// //   auto size = std::size_t(end - ifs.tellg());
// //   if (size == 0) return {};

// //   byte_arr buffer(size);
// //   if (!ifs.read((char*) buffer.data(), buffer.size())) {
// //     throw std::runtime_error(filepath + ": " + std::strerror(errno));
// //   }

// //   return buffer;
// // }
