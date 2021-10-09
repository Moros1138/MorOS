MBOOT_MAGIC     equ 0x1badb002
MBOOT_FLAGS     equ (1<<0 | 1<<1)
MBOOT_CHECKSUM  equ -(MBOOT_MAGIC + MBOOT_FLAGS)

[BITS 32]
[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

mboot:
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

    dd mboot
    dd code
    dd bss
    dd end
    dd start

[GLOBAL start]
[EXTERN _main]

start:
    push ebx
    
    cli
    call _main
    jmp $
