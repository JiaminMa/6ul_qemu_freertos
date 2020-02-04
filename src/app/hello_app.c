#include <print.h>
#include <task.h>

void hello_task2(void *pvParameters) 
{
    uint32_t i = 0;
    for (;;) {
        trace("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}

void hello_task1(void *pvParameters)
{
    uint32_t i = 0;
    for (;;) {
        trace("%s:%d\n", __func__, i++);
        vTaskDelay(1000);
    }
}
