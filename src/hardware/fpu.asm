; tell the assembler that we want to use 32-bit instructions
bits 32

;-------------------------------------------------------------------
; HELPING HANDS
;-------------------------------------------------------------------
%macro __func_entry 1
global %1
%1:
    push ebp
    mov ebp, esp
%endmacro

%macro __func_exit 0
    mov esp, ebp
    pop ebp
    ret
%endmacro

;-------------------------------------------------------------------
; SECTION .text (CODE)
;-------------------------------------------------------------------
section .text
align 4

;-------------------------------------------------------------------
; bool fpu_init();
;-------------------------------------------------------------------
__func_entry fpu_init

    mov dword [esp-4], 0xdeadbeef   ; inititailize it with garbage

    mov eax, cr0        ; start probe, get cr0
    and eax, (-1)-0xC   ; clear EM(bit 3) and TS(bit 4)
    mov cr0, eax        ; store control word
    
    fninit              ; initialize FPU to defaults
    fnstsw word [esp-4] ; store FPU status word
    cmp word [esp-4], 0 ; compare FPU status word
                        ; with an expected state
    
    ; if local dword != 0, no fpu
    jne .nofpu

    mov eax, 1      ; return true
    jmp .done       ; we're done

.nofpu:
    mov eax, 0      ; return false
    
.done:

__func_exit

;-------------------------------------------------------------------
; double fpu_atan(double x);
;-------------------------------------------------------------------
__func_entry fpu_atan
    fld qword [esp+8]   ; x
    fld1                ; st(0) = 1.0, st(1) = x, zzZ ...
    fpatan
__func_exit


;-------------------------------------------------------------------
; double fpu_atan2(double y, double x);
;-------------------------------------------------------------------
__func_entry fpu_atan2
    fld qword [esp+8]   ; y st(0) = y
    fld qword [esp+16]  ; x st(0) = x, st(1) = y
    fpatan

__func_exit

;-------------------------------------------------------------------
; double fpu_abs(double x);
;-------------------------------------------------------------------
__func_entry fpu_abs
    fld qword [esp+8]   ; x
    fabs                ; abs(x)
__func_exit

;-------------------------------------------------------------------
; double fpu_ceil(double x);
;-------------------------------------------------------------------
__func_entry fpu_ceil
    fld qword [esp+8]   ; load x into FPU st(0) register

    ; http://www.website.masmforum.com/tutorials/fptute/fpuchap1.htm
    mov al, 0x0B        ; IC=0 RC=10 PC=11 = 0x0b

    jmp round_common    ; jump to the routine shared by all rounding
                        ; functions

;-------------------------------------------------------------------
; double fpu_cos(double x);
;-------------------------------------------------------------------
__func_entry fpu_cos
    fld qword [esp+8]   ; x
    fcos                ; cos(x)
__func_exit

;-------------------------------------------------------------------
; double fpu_floor(double x);
;-------------------------------------------------------------------
__func_entry fpu_floor
    fld qword [esp+8]   ; load x into FPU st(0) register

    ; http://www.website.masmforum.com/tutorials/fptute/fpuchap1.htm
    mov al, 0x07        ; IC=0 RC=01 PC=11 = 0x07

    jmp round_common    ; jump to the routine shared by all rounding
                        ; functions

;-------------------------------------------------------------------
; double fpu_modulo(double x, double y);
;-------------------------------------------------------------------
__func_entry fpu_modulo
    fld qword [esp+16]  ; y
    fld qword [esp+8]   ; x
    fprem
__func_exit

;-------------------------------------------------------------------
; double fpu_pow(double x, double y);
;-------------------------------------------------------------------
__func_entry fpu_pow
    
    fld qword [esp+16] ; y   st0 = y, st1 = zzZ ...
    fld qword [esp+8]  ; x   st0 = x, st1 = y, st2 = zzZ ...
    
    fyl2x       ; st0 = y*log2(x), st1 = y, st2 = zzZ ...
    fld1        ; load 1.0 in st0
    
    fscale      ; (int)st1 + st0, at least that's what it would look like in C
    fxch st1    ; swap st0 and st1
    
    fld1        ; load 1.0 in st0
    fxch st1    ; swap st0 and st1
    
    fprem       ; partial remainder
    f2xm1       ; 2 to the x power, minus 1
    faddp       ; st0 + st1
    fmulp       ; st0 * st1

__func_exit

;-------------------------------------------------------------------
; double fpu_sin(double x);
;-------------------------------------------------------------------
__func_entry fpu_sin
    fld qword [esp+8]   ; x
    fsin                ; sin(x)
__func_exit

;-------------------------------------------------------------------
; double fpu_sqrt(double x);
;-------------------------------------------------------------------
__func_entry fpu_sqrt
    fld qword [esp+8]   ; x
    fsqrt               ; sqrt(x)
__func_exit

;-------------------------------------------------------------------
; double fpu_tan(double x);
;-------------------------------------------------------------------
__func_entry fpu_tan
    fld qword [esp+8]   ; x
    fptan               ; replace x with partial tan(x)
    
    fnstsw	ax          ; get FPU status word into ax
    
    ; if(ax & 0x400 != 0) then .1
    and ax, 0x400
    jnz	.1      
    
    fstp qword [eax]    ; pop x to eax
__func_exit

.1:
    fldpi               ; push pi onto fpu stack
    fadd                
    fxch
.2:
    fprem1
    
    fstsw	ax          ; get FPU status word into ax
    
    ; if(ax & 0x400 != 0) then .2
    and	ax, 0x400
    jnz	.2

    fstp
    fptan
    fstp qword [eax]    ; pop x to eax
__func_exit

;-------------------------------------------------------------------
; double fpu_trunc(double x);
;-------------------------------------------------------------------
__func_entry fpu_trunc
    fld qword [esp+8]   ; load x into FPU st(0) register

    ; http://www.website.masmforum.com/tutorials/fptute/fpuchap1.htm
    mov al, 0x0F        ; IC=0 RC=11 PC=11 = 0x0f

    jmp round_common    ; jump to the routine shared by all rounding
                        ; functions

;-------------------------------------------------------------------
; routine shared by all rounding functions (ceil, floor, trunc)
;-------------------------------------------------------------------
round_common:
    fstcw [esp+8]       ; recycle esp+8, no longer needed
                        ; because it's been pushed into st(0)

    mov ah, [esp+9]     ; this bit uses AH to backup the high
                        ; byte of the control word so we can
                        ; restore it later.
    
    mov [esp+9], al     ; AL set appropriately in the rounding
                        ; function before jumping into this
                        ; common routine
    
    fldcw [esp+8]       ; set the control word

    frndint             ; execute the rounding in the FPU
    
    mov [esp+9], ah     ; this bit restores the in the stack
    fldcw [esp+8]       ; and sets it.

__func_exit


;-------------------------------------------------------------------
; SECTION .bss (Read/Write reserved memory, uninitialized)
;-------------------------------------------------------------------
section .bss
align 16

