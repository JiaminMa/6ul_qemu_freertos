SECTIONS{
    . = 0x00000000;
    .text : 
    {
        obj/start.o
        *(.text)
    }
    . = 0x80000000;
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    . = . + 0x8000; /* 32kB of stack memory */
    svc_stack_top = .;
    . = . + 0x8000; /* 32kB of stack memory */
    irq_stack_top = .;
    . = . + 0x8000; /* 32kB of stack memory */
    sys_stack_top = .;
}
