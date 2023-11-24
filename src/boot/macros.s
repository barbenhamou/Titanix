%ifndef MACROS
%define MACROS

%define REAL_MODE_ADDR(addr) (addr) ;(addr - 0x7e00 + real_mode_start)

%define MMAP 0x8000
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

%macro print 1+
    jmp %%endstr 
    %%str: db %1
    %%endstr 
    %assign i 0
    %rep %%endstr-%%str
        printc byte [%%str+i]
        %assign i i+1
    %endrp
%endmacro

%macro println 1+
    print %1, 10, 13
%endmacro

%endif