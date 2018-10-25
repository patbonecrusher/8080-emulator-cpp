
MKPATH	= @[ -d "$(1)" ] || mkdir -p "$(1)"

OLD_WAY=0

APP       = emu8080.app
NEWAPP    = new_emu.app
SRC       = $(wildcard *.cpp)
OBJ       = $(SRC:.cpp=.o)

CPPFLAGS  = -std=gnu++17 # -Wall -Werror # -fPIC -O2 -g # -I./include
CPPFLAGS += -DOLD_WAY=${OLD_WAY}
CC        = g++

.PHONY: all clean

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

$(APP): $(OBJ)
	$(CC) -o $@ $^

$(NEWAPP): $(OBJ)
	$(CC) -o $@ $^

all: $(APP) $(OBJ)
	@echo "hello"

test: $(APP)
	./$(APP) ../roms/space-invaders/invaders 0

diag: $(APP)
	./$(APP) ../roms/cpudiag.bin 100

clean:
	rm -rf *.o *.so.* *.a *.la $(APP)