#include <iostream>
#include "cxxopts.hpp"
#include "finally.hpp"
#include "program.hpp"
#include "picosha2.hpp"

#include "cpu_state.hpp"
#include "cpu.hpp"

#include "libzippp.h"
using namespace libzippp;
using namespace std;

void dumpZip() {
ZipArchive zf("../roms/invaders.zip");
  zf.open(ZipArchive::READ_ONLY);

  vector<ZipEntry> entries = zf.getEntries();
  vector<ZipEntry>::iterator it;
  for(it=entries.begin() ; it!=entries.end(); ++it) {
    ZipEntry entry = *it;
    string name = entry.getName();
    int size = entry.getSize();

    cout << name << ": " << size << endl;
    // //the length of binaryData will be size
    // void* binaryData = entry.readAsBinary();

    // //the length of textData will be size
    // string textData = entry.readAsText();

    //...
  }

  zf.close();
  
}

extern void ReadFileIntoBufferAt(uint8_t* memory, char* filename, uint32_t offset);
extern void main_old (const char * fileName, int offset);

// cpu should be machine
typedef std::function<void(cpu&, std::string const&)> program_loader_fn_t;

void diagnostic_loader(cpu& core, std::string const& filename) {
  ReadFileIntoBufferAt(core.memory, (char*) filename.c_str(), 0x100);
  core.cpu_state.pc = 0x100;

  //Skip DAA test    
  core.memory[0x59c] = 0xc3; //JMP    
  core.memory[0x59d] = 0xc2;    
  core.memory[0x59e] = 0x05;    

  core.memory[0x06a1] = 0x76;
}

void invader_loader(cpu& core, std::string const& filename) {
  
}


void main_new(std::string const& fileName) {
  // Get a program from the factory
  // Tell the machine to run the program
  //    for now, load the program in the memory and start the cpu.

	int done = 0;
  system("/bin/stty -raw");
  auto resetTtyOnExit = onExit([&](){ printf("Y\n"); system("stty cooked"); system("stty sane"); });

  auto core = cpu();
  core.load_instruction_set();
  program_loader_fn_t loader_fn;

  if (fileName == "../roms/cpudiag.bin") {
    loader_fn = diagnostic_loader;
  }
  loader_fn(core, fileName);

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

    bool apple = false;
    std::string rom_path;
    std::string approach;

    options
      .allow_unrecognised_options()
      .add_options()
      ("r,roms", "roms location", cxxopts::value<std::string>(rom_path)->default_value("../roms"))
      ("a,approach", "old/new", cxxopts::value<std::string>(approach)->default_value("old"))
      // ("a,apple", "an apple", cxxopts::value<bool>(apple))
      ("b,bob", "Bob")
      ("t,true", "True", cxxopts::value<bool>()->default_value("true"))
      ("f, file", "File", cxxopts::value<std::vector<std::string>>(), "FILE")
      ("i,input", "Input", cxxopts::value<std::string>())
      ("o,output", "Output file", cxxopts::value<std::string>()
          ->default_value("a.out")->implicit_value("b.def"), "BIN")
      ("positional",
        "Positional arguments: these are the arguments that are entered "
        "without an option", cxxopts::value<std::vector<std::string>>())
      ("long-description",
        "thisisareallylongwordthattakesupthewholelineandcannotbebrokenataspace")
      ("help", "Print help")
      ("int", "An integer", cxxopts::value<int>(), "N")
      ("float", "A floating point number", cxxopts::value<float>())
      ("option_that_is_too_long_for_the_help", "A very long option")
    #ifdef CXXOPTS_USE_UNICODE
      ("unicode", u8"A help option with non-ascii: à. Here the size of the"
        " string should be correct")
    #endif
    ;

    options.add_options("Group")
      ("c,compile", "compile")
      ("d,drop", "drop", cxxopts::value<std::vector<std::string>>());

    options.parse_positional({"input", "output", "positional"});

    if (argc == 1) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    auto result = options.parse(argc, argv);

    std::cout << rom_path << std::endl;
    std::cout << approach << std::endl;

    // std::ifstream f(result["input"].as<std::string>().c_str(), std::ios::binary);
    // std::vector<unsigned char> s(picosha2::k_digest_size);
    // picosha2::hash256(f, s.begin(), s.end());
    // std::string hex_str = picosha2::bytes_to_hex_string(s.begin(), s.end());
    // std::cout << hex_str << std::endl;
    // dumpZip();

    if (approach == "old") {
      main_old(result["input"].as<std::string>().c_str(), 0x100);
      exit(0);
    } else if (approach == "new") {
      main_new(result["input"].as<std::string>().c_str());
      exit(0);
    }

    if (result.count("help"))
    {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    if (apple)
    {
      std::cout << "Saw option ‘a’ " << result.count("a") << " times " <<
        std::endl;
    }

    if (result.count("b"))
    {
      std::cout << "Saw option ‘b’" << std::endl;
    }

    if (result.count("f"))
    {
      auto& ff = result["f"].as<std::vector<std::string>>();
      std::cout << "Files" << std::endl;
      for (const auto& f : ff)
      {
        std::cout << f << std::endl;
      }
    }

    if (result.count("input"))
    {
      std::cout << "Input = " << result["input"].as<std::string>()
        << std::endl;
    }

    if (result.count("output"))
    {
      std::cout << "Output = " << result["output"].as<std::string>()
        << std::endl;
    }

    if (result.count("positional"))
    {
      std::cout << "Positional = {";
      auto& v = result["positional"].as<std::vector<std::string>>();
      for (const auto& s : v) {
        std::cout << s << ", ";
      }
      std::cout << "}" << std::endl;
    }

    if (result.count("int"))
    {
      std::cout << "int = " << result["int"].as<int>() << std::endl;
    }

    if (result.count("float"))
    {
      std::cout << "float = " << result["float"].as<float>() << std::endl;
    }

    std::cout << "Arguments remain = " << argc << argv[0] << std::endl;

  } catch (const cxxopts::OptionException& e)
  {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
