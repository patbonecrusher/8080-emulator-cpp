
MKPATH	= @[ -d "$(1)" ] || mkdir -p "$(1)"


APP       = emu8080.app
SRC       = $(wildcard *.cpp)
OBJ       = $(SRC:.cpp=.o)

CPPFLAGS  = -std=gnu++17 # -Wall -Werror # -fPIC -O2 -g # -I./include
CC        = g++

.PHONY: all clean

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

$(APP): $(OBJ)
	$(CC) -o $@ $^


all: $(APP) $(OBJ)
	@echo "hello"

test: $(APP)
	./$(APP) ../roms/space-invaders/invaders

clean:
	rm -rf *.o *.so.* *.a *.la $(APP)