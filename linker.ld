ENTRY(start_32)

SECTIONS {
    . = 0x100000;
    kernel_start = .;

    .text : ALIGN(4096) {
        *(.text)
    }

    .rodata : ALIGN(4096) {
        *(.rodata)
    }

    .data : ALIGN(4096) {
        *(.data)
    }

    .bss : ALIGN(4096) {
        bss_start = .;
        *(.bss)
        bss_end = .;
    }

    kernel_end = .;
}