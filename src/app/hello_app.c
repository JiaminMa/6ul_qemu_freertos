#include <app.h>

static void delay()
{
    uint32_t i,j = 0;
    for (i = 0; i < 1000; i++) 
        for (j = 0; j < 65536; j++);
}

void hello_task2(void *pvParameters) 
{
    uint32_t i = 0;
    for (;;) {
        trace("%s:%d\n", __func__, i++);
        // vTaskDelay(1000);
        delay();
    }
}

void hello_task1(void *pvParameters)
{
    uint32_t i = 0;
    for (;;) {
        trace("%s:%d\n", __func__, i++);
        // vTaskDelay(1000);
        delay();
    }
}

TaskHandle_t g_print_task_handle = NULL;

void print_task(void *pvParameters)
{
    while (1) {
        vTaskSuspend(g_print_task_handle);
        trace("print stask resumed\n");
    }
}
