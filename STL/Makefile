# QEMU OPTIONS
# QEMU_OPTS	:= -m 4G

DEBUG	:=
RELEASE := -O2 -s

BUILD	:= $(RELEASE)

# okay let's do some makefile fu!!
CC	:= i686-elf-gcc
AS	:= nasm -f elf32
AR	:= i686-elf-ar
LD	:= i686-elf-ld

BIN	:= bin
LIB := lib
INC	:= include
OBJ	:= obj
SRC	:= src

CCFLAGS	:= $(BUILD) -I$(INC) -I../STL/include -Wall -Wextra -Wpedantic -nostdlib -fno-builtin -fno-leading-underscore -Wno-write-strings -fno-rtti -fno-exceptions -fno-use-cxa-atexit -std=c++17
LDFLAGS	:= 

LIBRARY := $(LIB)/libc++.a

OBJECTS := \
	obj/moros_new.o

obj/%.o: src/%.cpp
	@echo CPP: $@
	@$(CC) $(CCFLAGS) -c $< -o $@

$(LIBRARY): $(OBJECTS)
	$(AR) rcs $(LIBRARY) $(OBJECTS)

clean:
	@echo Cleaning Artifacts
	@rm -f $(OBJECTS) $(LIBRARY)
