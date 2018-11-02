#pragma once
#include <functional>

class cpu;

typedef std::function<void(cpu&, std::string const&)> program_loader_fn_t;

program_loader_fn_t loader_for_file(std::string const& file_name);
