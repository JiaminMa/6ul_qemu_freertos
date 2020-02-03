/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_common.h"
#include <stdarg.h>
#include <stdint.h>
#include <print.h>

#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
static void hello_task(void *pvParameters);
static void hello_task2(void *pvParameters);

void *memcpy (void *__restrict __dest, const void *__restrict __src,
             size_t __n)
{
    uint8_t *dst = (uint8_t *)__dest;
    uint8_t *src = (uint8_t *)__src;
    while(__n--) {
        *dst++ = *src++;
    }
    return dst;
}

void *memset (void *__s, int __c, size_t __n)
{
    uint8_t *dst = (uint8_t *)__s;
    while(__n--)
        *dst++ = __c;
    return dst;
}


void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}

int main(void)
{

     GIC_Init();
    __set_VBAR((uint32_t)0);

    /* Install IRQ Handler */
    SystemInitIrqTable();
    if (true == print_init()) {
        trace("%s print init success\n ", __func__);
    } else {
        trace("%s failure system hangs...\n", __func__);
        while(1);
    }

    if (xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL) != pdPASS)
    {
        trace("Task creation failed!.\n");
        while (1)
            ;
    }

    if (xTaskCreate(hello_task2, "Hello_task2", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL) != pdPASS)
    {
        trace("Task creation failed!.\n");
        while (1)
            ;
    }
    vTaskStartScheduler();
    for (;;)
        ;
}

static void hello_task2(void *pvParameters)
{
    uint32_t i = 0;
    for (;;)
    {
        trace("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}

static void hello_task(void *pvParameters)
{
    uint32_t i = 0;
    for (;;)
    {
        trace("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}
