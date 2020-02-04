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

#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
static void hello_task(void *pvParameters);
static void hello_task2(void *pvParameters);
void epit_task(void *pvParamters);

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

int main(void)
{
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

    if (xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, 
                NULL, hello_task_PRIORITY, NULL) != pdPASS) {
        trace("Task creation failed!.\n");
        while (1)
            ;
    }

    if (xTaskCreate(hello_task2, "Hello_task2", configMINIMAL_STACK_SIZE + 10, 
                NULL, hello_task_PRIORITY, NULL) != pdPASS) {
        trace("Task creation failed!.\n");
        while (1)
            ;
    }

    if (xTaskCreate(epit1_task, "epit1_task", 128, 
                NULL, hello_task_PRIORITY, NULL) != pdPASS) {
        trace("Task creation failed!.\n");
        while (1)
            ;
    }

 
    vTaskStartScheduler();
    for (;;);
}

static void hello_task2(void *pvParameters)
{
    uint32_t i = 0;
    for (;;) {
        trace("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}

static void hello_task(void *pvParameters)
{
    uint32_t i = 0;
    for (;;) {
        trace("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}

