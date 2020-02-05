#include <stdarg.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <stdbool.h>
#include <semphr.h>
#include <fsl_common.h>
#include <task.h>
#include <fsl_debug_console.h>

void hello_task2(void *pvParameters) 
{
    uint32_t i = 0;
    for (;;) {
        PRINTF("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}

void hello_task1(void *pvParameters)
{
    uint32_t i = 0;
    for (;;) {
        PRINTF("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}
