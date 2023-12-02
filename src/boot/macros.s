%ifndef MACROS
%define MACROS

%define REAL_MODE_ADDR(addr) (addr) ;(addr - 0x7e00 + real_mode_start)

%define MMAP 0x5000
%define E820_MAGIC_NUM 0x0534d4150

%macro UpdateSelectors 1
    mov ax, %1
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
%endmacro

%macro printc 1
    push ax
    mov al, %1
    mov ah, 0x0e
    int 0x10
    pop ax
%endmacro

%macro prints 1+
    jmp %%endstr 
    %%str: db %1
    %%endstr 
    %assign i 0
    %rep %%endstr-%%str
        printc byte [%%str+i]
        %assign i i+1
    %endrep
%endmacro

%macro println 1+
    prints %1, 10, 13
%endmacro

%endif