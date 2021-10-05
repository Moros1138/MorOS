[BITS 32]
global _start

    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_HEADER_MAGIC  equ 0x1badb002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
    MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

SECTION .multiboot
ALIGN 4
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM


SECTION .bss
ALIGN 16
stack_bottom:
    resb 2*1024*1024 ; # 2 MiB
stack_top:

; code
SECTION .text
extern call_constructors
extern kmain


_start:
    mov esp, stack_top
    
    ; call our constructors
    call call_constructors

    ; push our multiboot and magic parameters
    push eax
    push ebx
    
    ; execute function kmain from kernel.cpp
    call kmain
    
    cli

loop_forever:
    hlt
    jmp loop_forever