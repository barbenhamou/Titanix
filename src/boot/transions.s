%include "src/boot/macros.s"

section .rodata
include "src/boot/gdt.s"

section .text
bits 32
ivt:
    dw 0x03ff ; Limit  0 - 15
    dq 0 ; Base 16 - 89

bits 32

protected_to_long:
    pop esi ; Ret addr into esi, will change it later.

    ; Move p4 addr to cr3.
    mov eax, p4_table
    mov cr3, eax

    ; Enable physical addr extension (PAE).
    mov eax, cr4
    or eax, (1 << 5) ; PAE bit
    mov cr4, eax

    ; Set to long-mode.
    mov ecx, 0xC0000080 ; MSR bits.
    rdmsr
    or eax, (1 << 8) ; MSR bits.
    wrmsr

    ; Enable paging.
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax

    lgdt [gdt.pointer]

    jmp gdt.code64:protected_to_long.long_mode

bits 64
    .long_mode:
        
        UpdateSelectors gdt.data64

        ; Returning the return addr.
        and rsi, 0xffffffff
        push rsi
        ret

bits 64
long_to_protected:
    pop rsi ; Ret addr into rsi, will change it later.

    push gdt.code32
    push REAL_MODE_ADDR(long_to_protected.comp_mode)
    retfq

bits 32
    .comp_mode:

    UpdateSelectors gdt.data32

    ; Disable paging.
    mov eax, cr0
    or eax, ~(1 << 31)
    mov cr0, eax

    ; Disable long-mode.
    mov ecx, 0xC0000080 ; MSR bits.
    rdmsr
    or eax, ~(1 << 8) ; MSR bits.
    wrmsr

    ; Disable physical addr extension (PAE).
    mov eax, cr4
    or eax, ~(1 << 5) ; PAE bit
    mov cr4, eax

    ; Returning the addr to stack.
    push esi
    ret

bits 32
protected_to_real:
    pop esi ; Ret addr into esi, will change it later.

    cli
    jmp gdt.code16: REAL_MODE_ADDR(protected_to_real.to_real)

bits 16
    .to_real:

    UpdateSelectors gdt.data16

    lidt [REAL_MODE_ADDR(ivt)]

    ; Disable protected.
    mov eax, cr0
    and eax, ~(1 << 0)
    mov cr0, eax

    jmp 0:REAL_MODE_ADDR(protected_to_real.real_mode)

    .real_mode:

    UpdateSelectors 0

    ; Returning the addr to stack.
    push si
    ret

bits 16
real_to_protected:
    pop si ; Ret addr into esi, will change it later.

    cli

    ; Enable protected.
    mov eax, cr0
    and eax, (1 << 0)
    mov cr0, eax

    jmp gdt.code32: REAL_MODE_ADDR(real_to_protected.protected_mode)

bits 32
    .protected_mode:

    UpdateSelectors gdt.data32

    ; Returning the addr to stack.
    and esi, 0xffff
    push esi
    ret