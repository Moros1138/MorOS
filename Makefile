# QEMU OPTIONS
# QEMU_OPTS	:= -m 4G

DEBUG	:=
RELEASE := -O2 -s

BUILD	:= $(RELEASE)

# okay let's do some makefile fu!!
CC	:= i686-elf-gcc
AS	:= nasm -f elf32
LD	:= i686-elf-ld

BIN	:= bin
LIB := /home/jon/opt/cross/lib/gcc/i686-elf/11.2.0
INC	:= include
OBJ	:= obj
SRC	:= src

CFLAGS	:= $(BUILD) -I$(INC) -Werror -Wall -Wextra -Wpedantic -nostdlib -fno-builtin -fno-leading-underscore -Wno-write-strings -std=c99
CCFLAGS	:= $(BUILD) -I$(INC) -Werror -Wall -Wextra -Wpedantic -nostdlib -fno-builtin -fno-leading-underscore -Wno-write-strings -fno-rtti -fno-exceptions -fno-use-cxa-atexit
LDFLAGS	:= -L$(LIB) -lgcc

KERNEL		:= $(BIN)/kernel.bin
IMAGE		:= $(BIN)/MorOS.iso

KERNEL_OBJS := \
	obj/start.o \
	obj/hardware/io.o \
	obj/hardware/cpu.o \
	obj/hardware/timer.o \
	obj/hardware/keyboard.o \
	obj/hardware/mouse.o \
	obj/hardware/vga.o \
	obj/hardware/serial.o \
	obj/stdlib/cppstub.o \
	obj/stdlib/string.o \
	obj/stdlib/random.o \
	obj/stdlib/printf.o \
	obj/mem.o \
	obj/event.o \
	obj/kernel.o 

.PHONY: dirs all

all: dirs $(KERNEL)

dirs:
	@mkdir -p obj/stdlib
	@mkdir -p obj/hardware
	@mkdir -p bin
	@mkdir -p logs

obj/%.o: src/%.asm
	@echo AS: $@
	@$(AS) $< -o $@

obj/%.o: src/%.c
	@echo CC: $@
	@$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.cpp
	@echo CPP: $@
	@$(CC) $(CCFLAGS) -c $< -o $@

# kernel
$(KERNEL): $(KERNEL_OBJS)
	@echo LD: linking $(KERNEL_OBJS)
	@$(LD) -o $(KERNEL) $^ -Tsrc/link.ld $(LDFLAGS) $(BUILD)

# create disk image
$(IMAGE): $(KERNEL)
	@echo Creating Disk Image
	@mkdir -p isodir/boot/grub
	@cp $(KERNEL) isodir/boot/MorOS.kernel
	@cp $(SRC)/grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o $(BIN)/MorOS.iso isodir 2> /dev/null
	@rm -rf isodir

# run the disk image in qemu
run: $(IMAGE)
	@echo Running Image
	@echo
	@qemu-system-i386 -drive format=raw,file=$(IMAGE) $(QEMU_OPTS) -chardev stdio,id=char0,mux=on,logfile=logs/$(shell date +"%Y%m%d.%T").serial.log,signal=on -serial chardev:char0 -mon chardev=char0

clean:
	@echo Cleaning Artifacts
	@rm -f $(KERNEL) $(IMAGE) $(KERNEL_OBJS)

clean-log:
	@echo Cleaning Logs
	@rm -f logs/*.log

install:
	cp $(KERNEL) /boot/MorOS.kernel
