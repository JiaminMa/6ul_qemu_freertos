#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include <stdarg.h>
#include <stdint.h>
#include <print.h>
#include <app.h>
#include <core_ca7.h>

typedef struct task_desc_tag {
    void (*entry)(void *);
    char *name;
    int prio;
    uint32_t stack_size;
} task_desc_t;

void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}

void data_section_init()
{
    uint32_t src = *(uint32_t *)0x40;
    uint32_t dst = *(uint32_t *)0x44;
    uint32_t len = *(uint32_t *)0x48 - *(uint32_t *)0x44;
    memcpy((void *)dst, (void *)src, len);
}

static task_desc_t tasks[] = {
    {
        hello_task1,
        "hello_task1",
        configMAX_PRIORITIES - 1,
        128,
    },

    {
        hello_task2,
        "hello_task2",
        configMAX_PRIORITIES - 1,
        128,
    }, 

    {
        epit1_task,
        "epit1_task",
        configMAX_PRIORITIES - 1,
        128,
    },

    {
        uart1_task,
        "uart1_task",
        configMAX_PRIORITIES - 1,
        128,
    },

    {
        NULL,
        NULL,
        0,
        0,
    }
};

int main(void)
{
    uint32_t i = 0;

    /* don't touch any global variable before here */
    data_section_init();

     GIC_Init();
    __set_VBAR((uint32_t)0);
    SystemInitIrqTable();

    if (true == print_init()) {
        trace("%s print init success\n ", __func__);
    } else {
        trace("%s failure system hangs...\n", __func__);
        while(1);
    }

    while (tasks[i].entry != NULL) {
        if (xTaskCreate(tasks[i].entry, 
                        tasks[i].name, 
                        tasks[i].stack_size,  
                        NULL, 
                        tasks[i].prio, 
                        NULL) != pdPASS) {
            trace("%s creation failed!.\n", tasks[i].name);
            while (1);
        } else {
             trace("%s creation success!.\n", tasks[i].name);
        }
        i++;
    }
 
    vTaskStartScheduler();
    for (;;);
}

