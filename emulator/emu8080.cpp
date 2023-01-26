// g++ -std=gnu++17 emu8080.cpp -o emu8080
// ./emu8080 roms/file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <cstddef>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "cpu_state.hpp"
#include "cpu.hpp"

using namespace std;
using std::cout;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;
// using std::byte;

// typedef vector<byte> bin_array;
// typedef bin_array::iterator bin_array_it;
// typedef bin_array::const_iterator bin_array_const_it;

extern void ReadFileIntoBufferAt(uint8_t* memory, char* filename, uint32_t offset)
{
	FILE *f= fopen(filename, "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	uint8_t *buffer = &memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

template<typename Function>
class Finally final
{
public:
    explicit Finally(Function f) : f(std::move(f)) {}
    ~Finally() { f(); } // (1) See below

    Finally(const Finally&) = delete;
    Finally(Finally&&) = default;
    Finally& operator =(const Finally&) = delete;
    Finally& operator =(Finally&&) = delete;
private:
    Function f;
};

template<typename Function>
auto onExit(Function &&f) { return Finally<std::decay_t<Function>>{std::forward<Function>(f)}; }

#include <termios.h>
#include <unistd.h>


extern void main_old (const char * fileName, int offset) {
  // termios oldt;
  // tcgetattr(STDIN_FILENO, &oldt);
  // termios newt = oldt;
  // newt.c_lflag &= ~ECHO;
  // tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // auto file = string(argv[1]);
  // auto fileContent = load_file(file);
  // std::cout << "File content size is: " << fileContent.size() << std::endl;
  // dissassemble8080Blob(fileContent);

  // auto fileName = argv[1];
  // int offset = strtol(argv[2], NULL, 16);
	int done = 0;
  system("/bin/stty -raw");

  auto core = cpu();
  core.load_instruction_set();
  core.cpu_state.pc = offset;
  ReadFileIntoBufferAt(core.memory, (char*) fileName, offset);

  //Skip DAA test
  core.memory[0x59c] = 0xc3; //JMP
  core.memory[0x59d] = 0xc2;
  core.memory[0x59e] = 0x05;

  core.memory[0x06a1] = 0x76;

// make sure we use the system one, not the brew one.
  system("/bin/stty raw -echo");
  auto resetTtyOnExit = onExit([&](){ printf("Y\n"); system("stty cooked"); system("stty sane"); });
//   try {
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
//   } catch (system_error& err) {
//     cout << "Systen was halted" << endl;
//   }

  // system("stty cooked");
  // newt.c_lflag |= ECHO;
  // tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  // // return core.run();
}
