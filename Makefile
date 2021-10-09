# Using a custom built cross-compiler
#
# https://wiki.osdev.org/GCC_Cross-Compiler
#
CC		:=	i686-elf-g++
# AS		:=	i686-elf-as -c
AS		:=	nasm -felf32
LD		:=	i686-elf-ld

# debug configuration
DEBUG	:=	-ggdb3 -Og

# release configuration
RELEASE	:= -O2

# which build configuration
# BUILD	:= $(DEBUG)
BUILD	:= $(RELEASE)

# directories
BIN		:=	bin
INC		:= 	include
OBJ		:=	obj
SRC		:=	src

# compiler flags
CFLAGS	:=	-I$(INC) $(BUILD) -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
CFLAGS	+=	-DPRINTF_DISABLE_SUPPORT_LONG_LONG

# linker flags
LDFLAGS	:=	

# kernel object files
KERNEL_OBJS	:=\
	$(OBJ)/loader.o \
	$(OBJ)/io.o \
	$(OBJ)/monitor.o \
	$(OBJ)/memory.o \
	$(OBJ)/random.o \
	$(OBJ)/main.o

# make all
all: dirs img

# create directories if they don't exist
dirs:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)

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
	rm -f $(BIN)/MorOS.iso $(BIN)/MorOS.kernel $(OBJ)/*.o
