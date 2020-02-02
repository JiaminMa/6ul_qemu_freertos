
.global _start

_start:
    ldr pc, =Reset_Handler
    ldr pc, =Undefined_Handler 
    ldr pc, =SVC_Handler
    ldr pc, =PreAbort_Handler
    ldr pc, =DataAbort_Handler
    ldr pc, =NotUsed_Handler
    ldr pc, =IRQ_Handler
    ldr pc, =FIQ_Handler

Reset_Handler:

    cpsid i 

    MRC p15, 0, r0, c1, c0, 0 
    bic r0, r0, #(1 << 12)      /* Disable I Cache */
    bic r0, r0, #(1 << 11)      /* Disable branch prediction */
    bic r0, r0, #(1 << 2)       /* Disable D Cache*/
    bic r0, r0, #(1 << 1)       /* Disable Alignment */
    bic r0, r0, #(1 << 0)       /* Disable MMU */
    MCR p15, 0, r0, c1, c0, 0



    /* 设置处理器进入IRQ模式 */
    mrs r0, cpsr        /* 读取cpsr到r0*/
    bic r0, r0, #0x1f   /* 清除cpsr的bit4-0*/
    orr r0, r0, #0x12   /* 使用IRQ模式*/
    msr cpsr, r0        /* 将r0写入到cpsr*/
    ldr sp, =irq_stack_top /* 设置IRQ模式下的sp*/

    /* 设置处理器进入SYS模式 */
    mrs r0, cpsr        /* 读取cpsr到r0*/
    bic r0, r0, #0x1f   /* 清除cpsr的bit4-0*/
    orr r0, r0, #0x1f   /* 使用SYS模式*/
    msr cpsr, r0        /* 将r0写入到cpsr*/
    ldr sp, =sys_stack_top /* 设置SYS模式下的sp*/

    /* 设置处理器进入SVC模式 */
    mrs r0, cpsr        /* 读取cpsr到r0*/
    bic r0, r0, #0x1f   /* 清除cpsr的bit4-0*/
    orr r0, r0, #0x13   /* 使用SVC模式*/
    msr cpsr, r0        /* 将r0写入到cpsr*/
    ldr sp, =svc_stack_top /* 设置SVC模式下的sp*/

    cpsie i             /* 打开IRQ */
    b main              /* 跳转到C语言main函数*/


/* 未定义指令中断服务函数 */
Undefined_Handler:
    ldr r0, =Undefined_Handler
    bx r0

/* SVC中断服务函数 */
SVC_Handler:  
    ldr r0, =SVC_Handler
    bx r0

/* 预取终止中断服务函数 */   
PreAbort_Handler:
    ldr r0, =PreAbort_Handler
    bx r0

/* 数据终止中断服务函数 */    
DataAbort_Handler:
    ldr r0, =DataAbort_Handler
    bx r0

/* 未使用 */    
NotUsed_Handler:
    ldr r0, =NotUsed_Handler
    bx r0

/* IRQ中断服务函数 */
IRQ_Handler:
	push {lr}					/* 保存lr地址 */
	push {r0-r3, r12}			/* 保存r0-r3，r12寄存器 */

	mrs r0, spsr				/* 读取spsr寄存器 */
	push {r0}					/* 保存spsr寄存器 */

	ldr r1, =0xA00000
    add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0XC]			/* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，
								 * GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */
	
	cps #0x13					/* 进入SVC模式，允许其他中断再次进去 */
	
	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irqhandler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中 */

	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}				
	str r0, [r1, #0X10]			/* 中断执行完成，写EOIR */

	pop {r0}						
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc */
	

/* FIQ中断服务函数 */
FIQ_Handler:
    ldr r0, =FIQ_Handler
    bx r0


