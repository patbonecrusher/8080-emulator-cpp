#ifdef NATIVE
#include <cstring>

#include "program_factory.hpp"
#include "picosha2.hpp"

#include "cpu.hpp"

#include "utils/libzippp.h"
using namespace libzippp;
using namespace std;

extern void ReadFileIntoBufferAt(uint8_t* memory, char* filename, uint32_t offset);

void default_loader(cpu& core, std::string const& filename) {
  ReadFileIntoBufferAt(core.memory, (char*) filename.c_str(), 0x100);
  core.cpu_state.pc = 0x100;
}

void diagnostic_loader(cpu& core, std::string const& filename) {
  ReadFileIntoBufferAt(core.memory, (char*) filename.c_str(), 0x100);
  core.cpu_state.pc = 0x100;

  //Skip DAA test    
  core.memory[0x59c] = 0xc3; //JMP    
  core.memory[0x59d] = 0xc2;    
  core.memory[0x59e] = 0x05;    

  // core.memory[0x06a1] = 0x76;
}

void invader_loader(cpu& core, std::string const& filename) {
  ZipArchive zf(filename);
  zf.open(ZipArchive::READ_ONLY);

  vector<ZipEntry> entries = zf.getEntries();
  vector<ZipEntry>::iterator it;
  for(it=entries.begin() ; it!=entries.end(); ++it) {
    ZipEntry entry = *it;
    string name = entry.getName();
    int size = entry.getSize();

    cout << name << ": " << size << endl;
    //the length of binaryData will be size
    auto binaryData = entry.readAsBinary();

    auto e = name.rfind("invaders.e")!=std::string::npos;
    auto f = name.rfind("invaders.f")!=std::string::npos;
    auto g = name.rfind("invaders.g")!=std::string::npos;
    auto h = name.rfind("invaders.h")!=std::string::npos;

    cout << e << " " << f << " " << g << " " << h << endl;

    if (h) { cout << "H"; memcpy (&core.memory[0x0000], binaryData, size); }
    if (g) { cout << "G"; memcpy (&core.memory[0x0800], binaryData, size); }
    if (f) { cout << "F"; memcpy (&core.memory[0x1000], binaryData, size); }
    if (e) { cout << "E"; memcpy (&core.memory[0x1800], binaryData, size); }

    // //the length of textData will be size
    // string textData = entry.readAsText();

    //...
  }

  zf.close();
}

std::string compute_sha(std::string const& file_name) {
  std::ifstream f(file_name, std::ios::binary);
  std::vector<uint8_t> s(picosha2::k_digest_size);
  picosha2::hash256(f, s.begin(), s.end());
  return picosha2::bytes_to_hex_string(s.begin(), s.end());
}

program_loader_fn_t loader_for_file(std::string const& file_name) {
  string sha = compute_sha(file_name);
  if      (sha == "8dd25da9f24a29cb3da7dca8bd302aef3c6fb1f24cc88072863b540c06d082c3" ) { return diagnostic_loader; }
  else if (sha == "5ca061feed2f17dbd73c27b56f4c1945817dda9de0410c161755bce5cffdbf36" ) { return invader_loader; }
  else    { return default_loader; }
  return nullptr;
}
#endif