%define REAL_MODE_ADDR(addr) (addr - 0x7e00 + real_mode_start)

%macro UpdateSelectors 1
    mov ax, %1
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
%eendmacro