ENTRY(_start)

SECTIONS {
    /* Text section */
    .text : {
        *(.text)
    }

    /* Read-only data section */
    .rodata : {
        *(.rodata)
    }

    /* Data section */
    .data : {
        *(.data)
    }

    /* BSS section */
    .bss : {
        *(.bss)
    }

    /* Stack section */
    .stack : {
        /* Set the stack pointer to the end of the memory region */
        . = ORIGIN(memory) + LENGTH(memory);
        *(.stack)
    }

    /* Memory region */
    memory : ORIGIN = 0x1000, LENGTH = 0x3F000;
}
