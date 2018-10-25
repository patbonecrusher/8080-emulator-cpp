#include <iostream>
#include "cxxopts.hpp"
#include "finally.hpp"
#include "program.hpp"


int main (int argc, char*argv[]) {
  try
  {
    cxxopts::Options options(argv[0], " - example command line options");
    options
      .positional_help("[optional args]")
      .show_positional_help();

    bool apple = false;
    std::string rom_path;

    options
      .allow_unrecognised_options()
      .add_options()
      ("r,roms", "roms location", cxxopts::value<std::string>(rom_path)->default_value("../roms"))
      ("a,apple", "an apple", cxxopts::value<bool>(apple))
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
