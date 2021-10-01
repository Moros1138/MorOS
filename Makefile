# Using a custom built cross-compiler
#
# https://wiki.osdev.org/GCC_Cross-Compiler
#
CC		:=	i686-elf-g++
AS		:=	i686-elf-as
LD		:=	i686-elf-ld

BIN		:=	bin
INC		:= 	include
OBJ		:=	obj
SRC		:=	src

# compiler flags
CFLAGS	:=	-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -I$(INC)

# linker flags
LDFLAGS	:=	-ffreestanding -O2 -nostdlib -lgcc

# kernel object files
KERNEL_OBJS	:=\
	$(OBJ)/boot.o \
	$(OBJ)/kernel.o

# make all
all: dirs img

# create directories if they don't exist
dirs:
	mkdir -p bin
	mkdir -p obj

# assemble!
$(OBJ)/%.o: $(SRC)/%.s
	$(AS) -c $< -o $@

# compile C
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# compile C++
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# link the kernel
kernel: $(KERNEL_OBJS)
	$(CC) -T $(SRC)/link.ld -o $(BIN)/MorOS.kernel $(LDFLAGS) $(KERNEL_OBJS)

# build the bootable iso image
img: kernel
	mkdir -p isodir/boot/grub
	cp $(BIN)/MorOS.kernel isodir/boot/MorOS.kernel
	cp $(SRC)/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BIN)/MorOS.iso isodir
	rm -rf isodir

# run the kernel in qemu
run: kernel
	qemu-system-i386 -kernel $(BIN)/MorOS.kernel

# run the iso image in qemu
run-img: img
	qemu-system-i386 -cdrom $(BIN)/MorOS.iso

clean:
	rm -f $(BIN)/MorOS.iso $(BIN)/MorOS.kernel $(OBJ)/*.o