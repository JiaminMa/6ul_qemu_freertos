#include "bsp_int.h"
#include <print.h>
#include <core_ca7.h>

static unsigned int irqNesting;

static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

void system_irqtable_init(void)
{
    unsigned int i = 0;

    irqNesting = 0;
    for(i = 0; i < NUMBER_OF_INT_VECTORS; i++ )
    {
        irqTable[i].irqHandler = default_irqhandler;
        irqTable[i].userParam = NULL;
    }

}

void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = userParam;
}

void int_init(void)
{
    GIC_Init();
    system_irqtable_init();
    __set_VBAR((uint32_t)0);
}

void system_irqhandler(unsigned int gicciar)
{
    uint32_t intNum = gicciar & 0x3ff;
    if((intNum == 1023) || (intNum >= NUMBER_OF_INT_VECTORS))
    {
        return;
    }

    irqNesting++;

    irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
    irqNesting--;
}


void default_irqhandler(unsigned int gicciar, void *userParam)
{
    while(1)
    {
        trace("%s\n", __func__);
    }
}
