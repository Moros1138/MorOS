/* variables to be used to generate the multiboot header */
.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1badb002
.set CHECKSUM,  -(MAGIC + FLAGS)

/* multiboot header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* read-write uninitialized data*/
.section .bss
.align 16
stack_bottom:
.space 2*1024*1024; # 2 MiB
stack_top:

/* code */
.section .text
.extern call_constructors
.extern kmain

.global _start
.type _start, @function
_start:
    /*
        set the stack pointer to the top of the location
        we set aside for the stack.
    */
    mov $stack_top, %esp

    /* call our constructors */
    call call_constructors

    push %eax
    push %ebx
    
    /* execute function kmain from kernel.cpp */
    call kmain

    /* clear interrupt flag */
    cli

    /* halt forever */
1:  hlt
    jmp 1b

.size _start, . - _start
