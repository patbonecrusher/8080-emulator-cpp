
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

CC        := g++
LD        := g++
CPPFLAGS  = -std=gnu++17
LDFLAGS   = -L/usr/lib/x86_64-linux-gnu
LDFLAGS  += -L/usr/local/Cellar/lzlib/1.10/lib
LDFLAGS  += -L/usr/local/lib

MODULES   := utils .
SRC_DIR   := $(addprefix ./,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst ./%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))

APP       = build/emu8080.app

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs $(APP)

$(APP): $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@ -llz -lzip 


checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

deps := $(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
