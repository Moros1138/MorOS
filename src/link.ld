ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    /*
        I have no idea what this is about.
        I think it's saying that starting
        at this point should be 1 megabyte
        into RAM? or perhaps the kernel image?

        edit: yea, the kernel image is only
        like 10K (so far), so there's no way
        in hell we're starting out at 1 meg,
        I'm thinking this is a location in
        memory
    */
    . = 1M;
    
    /* multiboot and kernel code */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }
    
    /* read-only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }
    
    /* read/write initialized data */
    .data BLOCK(4K) : ALIGN(4K)
    {
        start_ctors = .;
        KEEP(*( .init_array ));
        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
        end_ctors = .;

        *(.data)
    }
    
    /*
        read/write unintialized data
        this is where the stack lives
    */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /DISCARD/ :
    {
        *(.fini_array*)
        *(.comment)
    }
}