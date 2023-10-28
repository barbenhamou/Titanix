EXTERN start

section .multiboot_header

header_start:
    dd 0xe85250d6 ; magic.
    dd 0 ; architecture.
    dd header_end - header_start ; header length.
    dd 0x100000000 - (0xe85250d6 + 0 + header_end-header_start) ; checksum.

    entry_address_tag_start:
        dw 3 ; type
        dw 0 ; flags
        dd entry_address_tag_end - entry_address_tag_start ; size
        dd start ; entry address
        dd 0; padding
    entry_address_tag_end:

    ; end_tag
    dw 0
    dw 0
    dd 8
header_end:


