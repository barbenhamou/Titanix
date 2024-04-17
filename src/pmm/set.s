global set
section .text
bits 64
set:
    push rbp
    mov rbp, rsp
    pushfq
    push rcx
    mov rax, rsi
    mov rcx, rdx
    shr rcx, 3

    lop:
        mov [rdi], rax
        add rdi, 8
        dec rcx
        cmp rcx, 0
        jne lop

    pop rcx
    popfq
    pop rbp
    ret