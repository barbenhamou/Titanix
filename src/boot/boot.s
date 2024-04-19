[GLOBAL start]                  ; Kernel entry point.
[EXTERN main]
global p4_table
global p3_table
global p2_table

%define p4_table 0x60000
%define p3_table 0x61000
%define p2_table 0x62000

section .bss
    align 0x1000

    align 0x1000                                ; align at 4 bytes
    resb 0x2000
    kernel_stack:                               ; label points to beginning of memory




        

section .text
  bits 32            
  start:
    mov esp, kernel_stack

    mov eax, p3_table
    or eax, 1 << 0 | 1 << 1
    mov dword [p4_table+0], eax
    mov dword [p4_table+4], 0

    ; Point the first entry in the p3 table to the p2 table
    mov eax, p2_table
    mov edi, p3_table
    mov ecx, 8
    .map_p3_table:
        mov edx, eax
        or edx, 1 << 0 | 1 << 1
        mov dword [edi], edx
        mov dword [edi + 4], 0
        add eax, 0x1000
        add edi, 8
        loop .map_p3_table

    mov ecx, 8
    shl ecx, 9                  ; multiply by 512
    xor eax, eax                ; start address is 0
    mov ebx, 1 << 0 | 1 << 1 | 1 << 7
    mov edi, p2_table
    .map_p2_table:
        or eax, ebx
        mov dword [edi], eax
        mov dword [edi + 4], 0
        add edi, 8
        add eax, 0x200000
        loop .map_p2_table


    ; move page table address to cr3
    mov eax, p4_table
    mov cr3, eax

    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    or eax, 1 << 16
    mov cr0, eax
  
    ; long mode.
    lgdt [gdt.pointer]

    ; update selectors
    mov ax, gdt.data64
    mov ss, ax
    mov ds, ax
    mov es, ax
    
    ;jumping to long mode.
    jmp gdt.code64:long_mode_start

    bits 64
    long_mode_start:
        call main                   ; call our main() function.
        jmp $                       ; Enter an infinite loop, to stop the processor
                                    ; executing whatever rubbish is in the memory
                                    ; after our kernel!
section .rodata
%include "src/boot/gdt.s"
    ; gdt64:
    ;     dq 0
    ; .code: equ $ - gdt64
    ;     dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53)
    ; .data: equ $ - gdt64
    ;     dq (1<<44) | (1<<47) | (1<<41)
    ; .pointer:
    ;     dw $ - gdt64 - 1
    ;     dq gdt64
