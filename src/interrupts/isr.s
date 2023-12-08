%ifndef ISR__S 
%define ISR__S 

section .text
bits 64

global isr_stub_table
extern exception_handler

%macro isr_error 1
    isr%+%1:
        push %1
        jmp $+(isr_frame_assemble - $)
        ;call exception_handler
        ;iretq
%endmacro

%macro isr_no_error 1
    isr%+%1:
        push word 0
        push %1
        jmp $+(isr_frame_assemble - $)
        ;call exception_handler
        ;iretq
%endmacro

%macro pushall 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro pushallcr 0
    mov rax, cr0
    push rax
    mov rax, cr2
    push rax
    mov rax, cr3
    push rax
    mov rax, cr4
    push rax
%endmacro

%macro popallcr 0 
    pop rax
    mov cr4, rax
    pop rax
    mov cr3, rax
    pop rax
    mov cr2, rax
    pop rax
    mov cr0, rax
%endmacro

%macro popall 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

isr_frame_assemble:
    push rbp
    mov rbp, rsp
    pushall
    pushallcr

    mov ax, ds
    push rax
    push qword 0

    mov ax, 0x10
    mov ds, ax

    lea rdi,[rsp + 0x10]
    call $+(exception_handler - $)

    pop rax
    pop rax
    mov ds, ax
    mov es, ax
    popallcr
    popall
    pop rbp
    iretq

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

%endif