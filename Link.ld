ENTRY(_start)

MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 16K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 2K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
    
    .text : {
        . = ALIGN(4);
        KEEP(*(.vector_table))     
        . = ALIGN(4);
        KEEP(*(.text.startup*))    
        . = ALIGN(4);
        *(.text*)                  
        *(.rodata*)                
        . = ALIGN(4);
    } > FLASH

    
    .data : AT (ADDR(.text) + SIZEOF(.text)) {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM

    
    .bss (NOLOAD) : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    
    _sidata = LOADADDR(.data);
}
