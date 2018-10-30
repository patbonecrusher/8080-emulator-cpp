// #pragma once
// #include <string>
// #include <cstddef>
// #include <cstdint>
// #include <vector>
// #include <functional>

// typedef std::function<void(uint8_t*, std::string&)> program_loader_fn_t;

// class Program {

//   public:
//     Program(std::string const& filename, program_loader_fn_t loaderFunction)
//      : filename(std::move(filename))
//      , loaderFunction(std::move(loaderFunction)) {}
//     Program(const Program&) = delete;
//     Program(Program&&) = default;
//     Program& operator =(const Program&) = delete;
//     Program& operator =(Program&&) = delete;

//     void load_program_in_memory(uint8_t* memory) {
//       this.loaderFunction(memory, )
//     }
  
//   private:
//     std::string filename;
//     program_loader_fn_t loaderFunction;
// };