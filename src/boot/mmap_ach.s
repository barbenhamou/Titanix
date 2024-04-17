%include "src/boot/macros.s"

global load_mmap 

section .text 
bits 16
load_mmap:
        mov edi, MMAP+4
        xor ebx, ebx
        xor ebp, ebp
        mov dword edx, 0x0534D4150	
        mov eax, 0xe820
        mov dword [es:di + 20], 1
        mov ecx, 24
        int 0x15
        jc .failed
        mov edx, E820_MAGIC_NUM
        cmp eax, edx
        jne .failed
        test ebx, ebx
        je .failed
        jmp .entry_check
    
    .loop:
        mov eax, 0xe820
        mov dword [es:di + 20], 1
        mov ecx, 24
        int 0x15
        jc .done
        mov edx, E820_MAGIC_NUM
    
    .entry_check:
        jcxz .skip_entry
        cmp cl, 20
        jbe .valid
        test byte [es:di + 20], 1
        je .skip_entry
    
    .valid:
        mov ecx, [es:di + 8]
        or ecx, [es:di + 12]
        jz .skip_entry
        inc ebp
        add di, 24

    .skip_entry:
        test ebx, ebx
        jnz .loop

    .done:
        push ebx
        xor ebx, ebx
        mov word ebx, MMAP
        mov dword [ebx], ebp
        pop ebx
        clc
        ret

    .failed:
        stc
        ret
