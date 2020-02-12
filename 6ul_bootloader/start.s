.align 4
.global reset
.global jump_os
.global c_entry
.section .isr_vector
.text
reset:
	B reset_handler
	B .
	B .             //SVC
	B .
	B .
	B .
	B .             //IRQ
	B .             //FIQ


reset_handler:
    ldr r0, =0x00900000
    mcr p15,0,r0,c12,c0,0
    ldr sp, =svc_stack_top
    bl c_entry
    b .
 
jump_os:
    mov r1, #0
    mov r2, #0
    mov r3, #0
    mov r4, #0
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    mov r9, #0
    mov r10, #0
    mov r11, #0
    mov r12, #0
    mov r13, #0
    mov r14, #0
    blx r0