
MKPATH	= @[ -d "$(1)" ] || mkdir -p "$(1)"

OLD_WAY=0

APP       = emu8080.app
NEWAPP    = new_emu.app
SRC       = $(wildcard *.cpp)
OBJ       = $(SRC:.cpp=.o)

CPPFLAGS  = -std=gnu++17 # -Wall -Werror # -fPIC -O2 -g # -I./include
CC        = g++
LDFLAGS   = -L/usr/lib/x86_64-linux-gnu
LDFLAGS  += -L/usr/local/Cellar/lzlib/1.10/lib
LDFLAGS  += -L/usr/local/lib

.PHONY: all clean

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

$(APP): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ -llz -lzip 

all: $(APP) $(OBJ)
	@echo "hello"

test: $(APP)
	./$(APP) ../roms/space-invaders/invaders 0

diag: $(APP)
	./$(APP) -i ../roms/cpudiag.bin -a new

clean:
	rm -rf *.o *.so.* *.a *.la $(APP)
