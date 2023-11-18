%ifndef GDT
%define GDT

gdt:
.null:
    dq 0
.code64: equ $ - gdt
    dw 0xffff ; Limit 0 - 15, defines the maximum addressable unit.
    dw 0 ; Base 16 - 31, defines the beginning of the segment.
    db 0 ; Base 32 - 39.
    db (1 << 7) | (1 << 4) | (1 << 3) | (1 << 1) | (1 << 0) ;Access byte 40 - 47, premissions and stuff, 1|00|1|1|0|1|1. 
    db ((1 << 1) << 4) | (0x0f) ; Limit 48 - 51, Flags 52 - 55, long mode flag is on, (0|0|1|0) | (1111).
    db 0 ; Base ; 56 - 63.
.data64: equ $ - gdt
    dw 0xffff ; Limit 0 - 15, defines the maximum addressable unit.
    dw 0 ; Base 16 - 31, defines the beginning of the segment.
    db 0 ; Base 32 - 39.
    db (1 << 7) | (1 << 4) | (1 << 1) | (1 << 0) ;Access byte 40 - 47, premissions and stuff, 1|00|1|0|0|1|1. 
    db ((0 << 1) << 4) | (0x0f) ; Limit 48 - 51, Flags 52 - 55, long mode flag is on, (0|0|0|0) | (1111).
    db 0 ; Base 56 - 63.
.code32: equ $ - gdt
    dw 0xffff ; Limit 0 - 15, defines the maximum addressable unit.
    dw 0 ; Base 16 - 31, defines the beginning of the segment.
    db 0 ; Base 32 - 39.
    db (1 << 7) | (1 << 4) | (1 << 3) | (1 << 1) | (1 << 0) ;Access byte 40 - 47, premissions and stuff, 1|00|1|1|0|1|1. 
    db ((1 << 2) << 4) | (0x00) ; Limit 48 - 51, Flags 52 - 55, DB flag is on, (0|1|0|0) | (0000).
    db 0 ; Base ; 56 - 63.
.data32: equ $ - gdt
    dw 0xffff ; Limit 0 - 15, defines the maximum addressable unit.
    dw 0 ; Base 16 - 31, defines the beginning of the segment.
    db 0 ; Base 32 - 39.
    db (1 << 7) | (1 << 4) | (1 << 1) | (1 << 0) ;Access byte 40 - 47, premissions and stuff, 1|00|1|0|0|1|1. 
    db ((1 << 2) << 4) | (0x00) ; Limit 48 - 51, Flags 52 - 55, DB flag is on, (0|1|0|0) | (0000).
    db 0 ; Base ; 56 - 63.
.code16: equ $ - gdt
    dw 0xffff ; Limit 0 - 15, defines the maximum addressable unit.
    dw 0 ; Base 16 - 31, defines the beginning of the segment.
    db 0 ; Base 32 - 39.
    db (1 << 7) | (1 << 4) | (1 << 3) | (1 << 1) | (1 << 0) ;Access byte 40 - 47, premissions and stuff, 1|00|1|1|0|1|1. 
    db (0 << 4) | (0x0f) ; Limit 48 - 51, Flags 52 - 55, no flag is on, (0|0|0|0) | (1111).
    db 0 ; Base ; 56 - 63.
.data16: equ $ - gdt
    dw 0xffff ; Limit 0 - 15, defines the maximum addressable unit.
    dw 0 ; Base 16 - 31, defines the beginning of the segment.
    db 0 ; Base 32 - 39.
    db (1 << 7) | (1 << 4) | (1 << 1) | (1 << 0) ;Access byte 40 - 47, premissions and stuff, 1|00|1|0|0|1|1. 
    db (0 << 4) | (0x0f) ; Limit 48 - 51, Flags 52 - 55, no flag is on, (0|0|0|0) | (1111).
    db 0 ; Base ; 56 - 63.
.pointer: 
    dw .pointer - gdt - 1 ; Size of gdt - 1.
    dq gdt ; Offset.

    
%endif