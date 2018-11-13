
# MKPATH	= @[ -d "$(1)" ] || mkdir -p "$(1)"

# APP       = emu8080.app
# NEWAPP    = new_emu.app
# SRC       = $(wildcard *.cpp)
# OBJ       = $(SRC:.cpp=.o)

# CPPFLAGS  = -std=gnu++17 # -Wall -Werror # -fPIC -O2 -g # -I./include
# CC        = g++
# LDFLAGS   = -L/usr/lib/x86_64-linux-gnu
# LDFLAGS  += -L/usr/local/Cellar/lzlib/1.10/lib
# LDFLAGS  += -L/usr/local/lib

# .PHONY: all clean

# %.o: %.cpp
# 	$(CC) $(CPPFLAGS) -c $^ -o $@

# $(APP): $(OBJ)
# 	$(CC) $(LDFLAGS) -o $@ $^ -llz -lzip 

# all: $(APP) $(OBJ)
# 	@echo "hello"

# test: $(APP)
# 	./$(APP) ../roms/space-invaders/invaders 0

# diag: $(APP)
# 	./$(APP) -i ../roms/cpudiag.bin -a new

# clean:
# 	rm -rf *.o *.so.* *.a *.la $(APP)

CC        := em++
LD        := em++
CPPFLAGS  = -std=gnu++1z
CPPFLAGS += -DWASM_MAIN
#LDFLAGS   = -L/usr/lib/x86_64-linux-gnu
#LDFLAGS  += -L/usr/local/Cellar/lzlib/1.10/lib
#LDFLAGS  += -L/usr/local/lib
LDFLAGS  += -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=2 -s ASSERTIONS=2 -s EXTRA_EXPORTED_RUNTIME_METHODS="['cwrap', 'ccall']"
# LDFLAGS  += -s WASM=1 -O3 -s DISABLE_EXCEPTION_CATCHING=2 -s ASSERTIONS=2 -s EXTRA_EXPORTED_RUNTIME_METHODS="['cwrap', 'ccall']"
#LDFLAGS  += -s DISABLE_EXCEPTION_CATCHING=0

MODULES   := utils .
SRC_DIR   := $(addprefix ./,$(MODULES))
BUILD_DIR := $(addprefix build_wasm/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst ./%.cpp,build_wasm/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))
#INCLUDES  += -I/usr/local/include
#INCLUDES  += -I/usr/include
#INCLUDES  += -I/usr/include/x86_64-linux-gnu

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs build_wasm/emulator.js

build_wasm/emulator.js: $(OBJ)
	@cp index.html build_wasm/index.html
	$(LD) $(LDFLAGS) $^ -o $@ #-llz -lzip 


checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf build_wasm

deps := $(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
