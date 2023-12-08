%ifndef IDT_S
%define IDT_S

section .text
bits 64

global load_idt

load_idt:
    push rbp
    mov rbp, rsp
    pushfq
    cli
    lidt [rdi]
    popfq
    pop rbp
    ret

%endif