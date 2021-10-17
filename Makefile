# Using a custom built cross-compiler
#
# https://wiki.osdev.org/GCC_Cross-Compiler
#
CC		:=	i686-elf-g++
# AS		:=	i686-elf-as -c
AS		:=	nasm -felf32
LD		:=	i686-elf-ld

# directories
BIN		:=	bin
INC		:= 	include
OBJ		:=	obj
SRC		:=	src

# compiler flags
CFLAGS	:=	-I$(INC) -Werror -Wall -Wpedantic -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings

# linker flags
LDFLAGS	:=	

# kernel object files
KERNEL_OBJS	:=\
	$(OBJ)/loader.o \
	$(OBJ)/descriptors/gdt.o \
	$(OBJ)/descriptors/idt.o \
	$(OBJ)/isr.o \
	$(OBJ)/io.o \
	$(OBJ)/hardware/monitor.o \
	$(OBJ)/hardware/timer.o \
	$(OBJ)/hardware/keyboard.o \
	$(OBJ)/hardware/mouse.o \
	$(OBJ)/memory.o \
	$(OBJ)/random.o \
	$(OBJ)/main.o

# make all
all: dirs img

# create directories if they don't exist
dirs:
	mkdir -p $(BIN) $(OBJ)/hardware $(OBJ)/descriptors

# assemble!
$(OBJ)/%.o: $(SRC)/%.asm
	$(AS) $< -o $@

# compile C
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# compile C++
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.txt
	$(LD) -r -b binary $< -o $@

# link the kernel
kernel: $(KERNEL_OBJS)
	$(LD) -T $(SRC)/link.ld -o $(BIN)/MorOS.kernel $(LDFLAGS) $(KERNEL_OBJS)

# build the bootable iso image
img: dirs kernel
	mkdir -p isodir/boot/grub
	cp $(BIN)/MorOS.kernel isodir/boot/MorOS.kernel
	cp $(SRC)/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BIN)/MorOS.iso isodir
	rm -rf isodir

# run the kernel in qemu
run: dirs kernel
	qemu-system-i386 -kernel $(BIN)/MorOS.kernel

# run the iso image in qemu
run-img: dirs img
	qemu-system-i386 -cdrom $(BIN)/MorOS.iso

clean:
	rm -f $(BIN)/MorOS.iso $(BIN)/MorOS.kernel $(KERNEL_OBJS)
