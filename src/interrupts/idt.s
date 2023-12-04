%ifndef IDT_S
%define IDT_S

section .text
bits 64

global isr_stub_table

global load_idt

extern exception_handler

%macro isr_error 1
    isr%+%1:
        call exception_handler
        iretq
%endmacro

%macro isr_no_error 1
    isr%+%1:
        call exception_handler
        iretq
%endmacro

isr_no_error 0
isr_no_error 1
isr_no_error 2
isr_no_error 3
isr_no_error 4
isr_no_error 5
isr_no_error 6
isr_no_error 7
isr_error 8
isr_no_error 9
isr_error 10
isr_error 11
isr_error 12
isr_error 13
isr_error 14
isr_no_error 15
isr_no_error 16
isr_error 17
isr_no_error 18
isr_no_error 19
isr_no_error 20
isr_no_error 21
isr_no_error 22
isr_no_error 23
isr_no_error 24
isr_no_error 25
isr_no_error 26
isr_no_error 27
isr_no_error 28
isr_no_error 29
isr_error 30
isr_no_error 31

isr_stub_table:
    %assign i 0
    %rep 32
        dq isr%+i
        %assign i i+1
    %endrep

load_idt:
    mov ebx, [rsp + 8]
    lidt [ebx]
    sti
    ret

%endif