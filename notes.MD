# Instruction
* mnemonic (its name)
* length (in bytes)
* duration (in cyles) (sometime min/max)
* flags
* code

## Instruction types
* misc/control instruction
* jump/calls
* 8 bits load/store/move
* 16 bits load/store/move
* 8 bits arithmetic/logical
* 16 bits arithmetics/logical

2MHZ = 2,000,000 clock cycles per second


http://www.shaels.net/index.php/cpm80-22-documents/cpm-bdos/36-bdos-file-io-services
The bdos instructions are used by the call 0xCD instruction.
in register C you put the function code you want to execute
this is a CP/M thing.   for instance function code 9 will output the $ terminated string at address stored in DE
http://www.shaels.net/index.php/cpm80-22-documents/cpm-bdos/31-bdos-overview


Flags
The processor maintains internal flag bits (a status register), which indicate the results of arithmetic and logical instructions. The flags are:

Sign (S), set if the result is negative.
Zero (Z), set if the result is zero.
Parity (P), set if the number of 1 bits in the result is even.
Carry (C), set if the last addition operation resulted in a carry or if the last subtraction operation required a borrow
Auxiliary carry (AC or H), used for binary-coded decimal arithmetic (BCD).

The carry bit can be set or complemented by specific instructions. Conditional-branch instructions test the various flag status bits. The flags can be copied as a group to the accumulator. The A accumulator and the flags together are called the PSW register, or program status word.`


The CPU step method executes the next instruction and return the number of cycles spent
Allowing the CPU to wait some.

There is a way to figure out the clock cycle of a computer and from that, adjust the game speed.  We can run the cpu in slow mode using thread affinity, which differs from os.

We know the CPU is 2MGZ, 2M clock cycles per second
We know how many clock cycles were interpreted

## Space Invader

### Video rendering
Video refreshing is set by specifying the fps.  For instance, 60fps = 16ms (every 960 ms)
Use a timer to trigger a refresh.

### Keystroke/Joystick
Uses in/out port.
Capture the keys up and down and tell the machine that they were pressed.
The machine can then handle the db and d3 requests.

### Sounds
Driven by the d3 request (OUT PORT).  Based on some values, output the proper file.

### Interrupt
Need to understand the interrupt that the game fires and when.

if (irq) ( execute instruction(interruptVector))

### CPU throttling
CPU is at 2MHz so 2M cycles/sec
Run the CPU in it's own thread
```
  double sinceLast = now - lastTimer;
  int cycles_to_catch_up = 2 * sinceLast;
  int cycles = 0;
  while (cycles_to_catch_up > cycles) {}

  // opt2

```


### Color layer
Look at the example from emulator 101

## Some C++ sample

```
#include <iostream>
#include <string>

template<typename writer>
class logger: public writer {
public:
  void warn(const std::string& msg){ this->write("WARN", msg); }
  void error(const std::string& msg){ this->write("ERROR", msg); }
  void info(const std::string& msg){ this->write("INFO", msg); }
};

class file_writer {
  /*...*/
public:
  void init( const std::string& file ) { /*...*/ }

protected:
  void write( const std::string& kind, const std::string& msg ) {
    /*...*/
  }
};

class network_writer {
  /*...*/
public:
  void init( const std::string& address, unsigned short port ) { /*...*/ }

protected:
  void write( const std::string& kind, const std::string& msg ) {
    /*...*/
  }
};

typedef logger<file_writer> file_logger;
typedef logger<network_writer> network_logger;
```