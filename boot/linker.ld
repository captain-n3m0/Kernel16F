ENTRY(start)

SECTIONS {
    . = 0x7c00;

    .text :
    {
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    . = 0x8000;
    .kernel :
    {
        *(.kernel)
    }

    /DISCARD/ :
    {
        *(.comment)
    }
}
