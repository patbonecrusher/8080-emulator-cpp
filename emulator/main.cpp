#include <iostream>
#include "cxxopts.hpp"
#include "finally.hpp"

#include "cpu_state.hpp"
#include "cpu.hpp"
#include "program_factory.hpp"

#ifndef WASM_MAIN
extern void main_old (const char * fileName, int offset);

void main_new(std::string const& file_name) {
	int done = 0;
  system("/bin/stty -raw");
  auto resetTtyOnExit = onExit([&](){ printf("Y\n"); system("stty cooked"); system("stty sane"); });

  auto core = cpu();
  core.load_instruction_set();
  program_loader_fn_t loader_fn = loader_for_file(file_name);

  loader_fn(core, file_name);

  system("/bin/stty raw -echo"); 
  try {
    char c = 'n';
    do {
      core.next();
      cout << "\r";

      c = getchar();
      if (c == 'g') {
        system("/bin/stty -raw");
        core.run();
      }
    } while (c != 's');
  } catch (system_error& err) {
    cout << "Systen was halted" << endl;
  }

}

int main (int argc, char*argv[]) {
  try
  {
    cxxopts::Options options(argv[0], " - example command line options");
    options
      .positional_help("[optional args]")
      .show_positional_help();

    std::string rom_path;
    std::string approach;

    options
      .allow_unrecognised_options()
      .add_options()
      ("r,roms", "roms location", cxxopts::value<std::string>(rom_path)->default_value("../roms"))
      ("a,approach", "old/new", cxxopts::value<std::string>(approach)->default_value("old"))
      ("i,input", "Input", cxxopts::value<std::string>())
      ("help", "Print help")
    ;

    options.parse_positional({"input"});

    if (argc == 1) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    auto result = options.parse(argc, argv);

    std::cout << rom_path << std::endl;
    std::cout << approach << std::endl;

    std::string file_name = result["input"].as<std::string>();
    if (approach == "old") {
      main_old(file_name.c_str(), 0x100);
    } else if (approach == "new") {
      main_new(file_name);
    }

  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}

#else

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

extern "C" unsigned char ___roms_cpudiag_bin[];
extern "C" unsigned int ___roms_cpudiag_bin_len;
using namespace emscripten;

int main (int argc, char*argv[]) {
  printf("hi\n");
}

struct Interface {
    virtual ~Interface() {}
    virtual void invoke(const std::string& str) = 0;
};

struct InterfaceWrapper : public wrapper<Interface> {
    EMSCRIPTEN_WRAPPER(InterfaceWrapper);
    void invoke(const std::string& str) {
        return call<void>("invoke", str);
    }
};

EMSCRIPTEN_BINDINGS(interface) {
    class_<Interface>("Interface")
        .function("invoke", &Interface::invoke, pure_virtual())
        .allow_subclass<InterfaceWrapper>("InterfaceWrapper")
        ;
}

Interface* iii;

extern "C" void EMSCRIPTEN_KEEPALIVE run_diag() {
  auto core = cpu();
  core.load_instruction_set();

  memcpy(&(core.memory[0x100]), ___roms_cpudiag_bin, ___roms_cpudiag_bin_len);
  core.cpu_state.pc = 0x100;

  //Skip DAA test    
  core.memory[0x59c] = 0xc3; //JMP    
  core.memory[0x59d] = 0xc2;    
  core.memory[0x59e] = 0x05;    

//  core.memory[0x06a1] = 0x76;
  try {
    iii->invoke("titicaca");
    iii->invoke("titicooo");
    core.run();
  } catch (system_error& err) {
    cout << "Systen was halted" << endl;
  } catch (std::exception& ex) {
    cout << "Systen was halted 2" << endl;
  }
}

struct Point2f {
    float x;
    float y;
};

struct PersonRecord {
    std::string name;
    int age;
};

PersonRecord findPersonAtLocation(Point2f) {
  PersonRecord p;
  p.name = "pat";
  p.age = 234;
  return p;
}

EMSCRIPTEN_BINDINGS(my_value_example) {
    value_array<Point2f>("Point2f")
        .element(&Point2f::x)
        .element(&Point2f::y)
        ;

    value_object<PersonRecord>("PersonRecord")
        .field("name", &PersonRecord::name)
        .field("age", &PersonRecord::age)
        ;

    emscripten::function("findPersonAtLocation", &findPersonAtLocation);
}


void invokeInterface(Interface* i) {
  printf("here\n");
  i->invoke("poopoo");
  iii = i;
}

EMSCRIPTEN_BINDINGS(infokeinf) {
    emscripten::function("invokeInterface", &invokeInterface, allow_raw_pointers());
}


#endif