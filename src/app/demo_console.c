#include <app.h>

TaskHandle_t scanfTaskHandle = NULL;

void print_task(void *pvParameters)
{
    while (1)
    {
        PRINTF("\r\nDebug console Demo-print task3.\r\n");
        /* delay */
        vTaskDelay(300U);
    }
}

void scanf_task(void *pvParameters)
{
    char ch;

    PRINTF("\r\nDebug console Demo-scanf task.\r\n"); 
    while(1) {
        PRINTF("\r\n Please Input a char together with press enter: \r\n");
        SCANF("%c", &ch);
        PUTCHAR(ch);
        taskYIELD();
    }
    /* suspend */
}
