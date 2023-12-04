%ifndef IDT_S
%define IDT_S

section .text
bits 64

global load_idt

load_idt:
    mov ebx, [rsp + 8]
    lidt [ebx]
    sti
    ret

%endif