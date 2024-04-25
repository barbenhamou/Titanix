ENTRY(start)

SECTIONS {
    . = 0xffffffff80000000 + 2M;

    load_base = .;

    .boot :
    {
        /* ensure that the multiboot header is at the beginning */
    }

    .text :
    {
        KEEP( *(.multiboot_header))
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .bss :
    {
        *(.bss)
    }

    .data :
    {
        *(.data)
    }

    load_max = .;
}