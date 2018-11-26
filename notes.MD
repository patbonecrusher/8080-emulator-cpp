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
Or everytime memory is updated in the region of space that is video, refresh the screen.  Not very efficient

### Keystroke/Joystick
Uses in/out port.
Capture the keys up and down and tell the machine that they were pressed.
The machine can then handle the db and d3 requests.

### Sounds
Driven by the d3 request (OUT PORT).  Based on some values, output the proper file.

### Interrupt
Need to understand the interrupt that the game fires and when.
Interrupt could be handled on a separate thread using a timer to be accurate. Would a faster clock affect the interrupt rate?
The interrupt actually jump the code to a specific address.  The video driver would have been the one who would have toggle the interrupts hardware wise.
The current PC is stored on the Stack pointer.  When the interrupt returns, it returns back to the PC and resume.
```
int_num = 1 or 2.
void GenerateInterrupt(State8080* state, int interrupt_num)
{
	//perform "PUSH PC"
    Push(state, (state->pc & 0xFF00) >> 8, (state->pc & 0xff));
	
	//Set the PC to the low memory vector
	state->pc = 8 * interrupt_num;
    
    //"DI"
    state->int_enable = 0;
}
```

if (irq) ( execute instruction(interruptVector))

#### Game Timing
The game-play timing is based on an interrupt from the video system. Actually there are two interrupts. When the video rendering reaches the middle of the screen an RST 1 (jumps to 0x0008) is generated. When the rendering reaches the end of the screen (when vertical blanking begins) an RST 2 (jumps to 0x0010) is generated.

The separate interrupts allow the code to keep out of the way of the monitor's electron beam. If an object is moved in memory while the rendering hardware is drawing from the same memory then part of the image will be drawn at the old place and part at the new. This makes a very brief visual glitch that could, in theory, be noticed by the player. The flickers accumulate in the players mind over time.

Objects at the top of the screen are drawn after the mid-screen interrupt. Objects at the bottom of the screen are drawn after the end-screen interrupt. Thus the code and hardware work on different halves of the screen. I'll talk more about the mechanics of this in Game Objects below.

*The screen refresh rate is 60Hz. So each interrupt is executed 60 times a second. (every 16.666667ms)* Each interrupt has a list of chores to perform described below.

##### End of Screen Interrupt
The end-screen interrupt checks the tilt switch. It counts the credits as coins are inserted. The limit is 99 stored as a byte of binary-coded-decimal. After that coins are just ignored (an overflow would wipe out $25 of quarters).

This interrupt decreases a general-purpose counter at 0x20C0. This counter is used to time small delays in the attract mode. The print-text code sets this timer and waits for it to reach zero before moving to the next letter. Thus the letters appear on the screen in a slow, steady beat.

This interrupt also counts the aliens and changes the rate of the "step" sound. The sound rate is independent of the actual steps, as I'll explain below in The Aliens.

There are three different alien shots, each with a different picture. The end-screen interrupt sets a flag to keep all three in sync so that only one shot is processed per screen refresh.

This interrupt code draws exactly one alien every interrupt. This interrupt code keeps up with the time between flying saucers.

And most importantly, this interrupt (like the mid-screen interrupt) gives all five game objects a chance to run. Each object decides which interrupt to use based on which half of the screen it is on. I'll discuss this in detail in Game Objects below.

##### Mid Screen Interrupt
The mid-screen interrupt handles attract-mode tasks. In the attract mode there are two different animations to amuse a potential player passing by the cabinet. The first is an upside down "Y" in the message "PLAY" above "SPACE INVADERS". The small alien drags the letter off the screen and brings back the correct one. The second is an extra "C" in "INSERT CCOIN". The small alien shoots the extra "C". These animations happen every-other pass through the attract mode and not on the first pass.

The mid-screen interrupt advances the "next alien" cursor for the draw-alien process managed by the end-screen interrupt. See The Aliens below.

And most importantly again, this interrupt (like the end-screen interrupt) gives all five game objects a chance to run. Each object decides which interrupt to use based on which half of the screen it is on. I'll discuss this in detail in Game Objects below.

### CPU throttling
CPU is at 2MHz so 2M cycles/sec
Run the CPU in it's own thread
```
while(1) {
  double sinceLast = now - lastTimer;     // measure amount passed since last timer.
  int cycles_to_catch_up = 2 * sinceLast;
  int cycles = 0;
  while (cycles_to_catch_up > cycles) {}
}

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