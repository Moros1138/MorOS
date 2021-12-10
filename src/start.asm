; tell the assembler that we want to use 32-bit instructions
bits 32

;-------------------------------------------------------------------
; MULTIBOOT CONSTANTS
;-------------------------------------------------------------------
MULTIBOOT_MAGIC     equ 0x1badb002
MULTIBOOT_ALIGNMENT equ 1 << 0
MULTIBOOT_MEMINFO   equ 1 << 1
MULTIBOOT_FLAGS     equ MULTIBOOT_ALIGNMENT | MULTIBOOT_MEMINFO
MUTLIBOOT_CHECKSUM  equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

;-------------------------------------------------------------------
; SYMBOLS
;-------------------------------------------------------------------

; c/c++ entry point
extern kernel_setup
extern main

; multiboot entrypoint
global _start

; gdt/idt routines
global gdt_flush
global idt_flush

; isr routines
extern isr_handler
extern irq_handler

;-------------------------------------------------------------------
; SECTION .multiboot (the code section of the file)
;-------------------------------------------------------------------
section .multiboot
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MUTLIBOOT_CHECKSUM

;-------------------------------------------------------------------
; SECTION .text (the code section of the file)
;-------------------------------------------------------------------
section .text
align 4

_start:
    
    ; setup stack
    mov esp, stack_top
    
    ; magic
    push eax 
    
    ; pointer to multiboot information structure
    push ebx 

    ; disable interrupts pre c/c++
    cli

    call kernel_setup
    
    ; ensure hardware was set up properly
    cmp eax, 0x3badb002
    jne die

    call main
    
die:
    cli     ; disable interrupts
    hlt     ; halt the cpu
    jmp $   ; loop here forever, if we make it here


gdt_flush:
    ; get the GDT pointer
    mov eax, [esp+4]
    ; load the GDT
    lgdt [eax]

    ; set up the segments to the new code segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush

.flush:
    ret

idt_flush:
    ; get IDT pointer
    mov eax, [esp+4]
    ; load the IDT
    lidt [eax]
    
    ; enable interrupts
    sti
    ret

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_handler_common
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    push byte %1
    jmp isr_handler_common
%endmacro

ISR_NOERRCODE  0
ISR_NOERRCODE  1
ISR_NOERRCODE  2
ISR_NOERRCODE  3
ISR_NOERRCODE  4
ISR_NOERRCODE  5
ISR_NOERRCODE  6
ISR_NOERRCODE  7
ISR_ERRCODE    8
ISR_NOERRCODE  9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31


isr_handler_common:
    
    pusha
    
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8
    sti
    iret

%macro IRQ 2
global irq%1
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_handler_common
%endmacro

IRQ  0, 32
IRQ  1, 33
IRQ  2, 34
IRQ  3, 35
IRQ  4, 36
IRQ  5, 37
IRQ  6, 38
IRQ  7, 39
IRQ  8, 40
IRQ  9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

irq_handler_common:
    pusha

    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    cld

    call irq_handler

    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8
    sti
    iret

;-------------------------------------------------------------------
; SECTION .data (read/write data)
;-------------------------------------------------------------------
section .data
align 4

;-------------------------------------------------------------------
; SECTION .bss (read/write unintialized data)
;-------------------------------------------------------------------
section .bss
align 16

stack_bottom:
    resb 16384
stack_top:
