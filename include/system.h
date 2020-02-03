#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <MCIMX6Y2.h>

typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler;
    void *userParam;
}sys_irq_handle_t;

void system_interrupt_init(void);
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);
void system_default_irqhandler(unsigned int gicciar, void *userParam);
#endif