%ifndef REAL_MODE
%define REAL_MODE

%include "src/boot/macros.s"

global real_mode_start
global real_mode_end
global transfer_to_real

section .text
bits 64

transfer_to_real:
    mov rsi, REAL_MODE_ADDR(long_to_protected)
    call rsi
    bits 32
    mov esi, REAL_MODE_ADDR(protected_to_real)
    call esi
    bits 16
    pusha
    call di
    popa
    mov si, REAL_MODE_ADDR(real_to_protected)
    call si
    bits 32
    mov esi, REAL_MODE_ADDR(protected_to_long)
    call esi
    bits 64

    ret

real_mode_start:
%include "src/boot/transions.s"
real_mode_end:

%endif