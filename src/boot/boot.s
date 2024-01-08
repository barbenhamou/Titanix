[GLOBAL start]                  ; Kernel entry point.
global p4_table
global p3_table
global p2_table
global p1_table

section .bss
    align 0x1000                                ; align at 4 bytes
    resb 0x2000
    kernel_stack:                               ; label points to beginning of memory

    align 0x1000
    p4_table:
        resb 0x1000
    p3_table:
        resb 0x1000
    p2_table:
        resb 0x1000
    p1_table:
        resb 0x1000


        

section .text
  bits 32
  [EXTERN main]                   ; This is the entry point of our C code
  start:
    mov esp, kernel_stack
    push ebx 
    ;Paging
    mov eax, p3_table
    or eax, 0b11 ;
    mov dword [p4_table + 0], eax

    ; Point the first entry of the level 3 page table to the first entry in the
    ; p2 table
    mov eax, p2_table
    or eax, 0b11
    mov dword [p3_table + 0], eax

    mov eax, p1_table
    or eax, 0b11
    mov dword [p2_table + 0], eax

    ; point each page table level one entry to a page
    mov ecx, 0         ; counter variable
    .map_p1_table:
        mov eax, 0x1000  ; 1KiB
        mul ecx
        or eax, 0b11
        mov [p1_table + ecx * 8], eax

        inc ecx
        cmp ecx, 512
        jne .map_p1_table

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
