#include <app.h>

typedef struct task_desc_tag {
    void (*entry)(void *);
    char *name;
    int prio;
    uint32_t stack_size;
    TaskHandle_t *task_handle_p;
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

void uart_task(void *pvParameters);

static task_desc_t tasks[] = {
    {hello_task1, "hello_task1", configMAX_PRIORITIES - 1, 128, NULL},
    {hello_task2, "hello_task2", configMAX_PRIORITIES - 1, 128, NULL},
    {
        epit1_task,
        "epit1_task",
        configMAX_PRIORITIES - 1,
        128,
    },
#if 0
    { print_task, "print_task", configMAX_PRIORITIES - 1, 128, NULL},
    { scanf_task, "scanf_task", configMAX_PRIORITIES - 1, 128, &
    scanfTaskHandle},
#endif
    
    {NULL, NULL, 0, 0, NULL,}
};

int main(void)
{
    uint32_t i = 0;

    /* don't touch any global variable before here */
    data_section_init();

    /* init the system interrupt */
     GIC_Init();
    __set_VBAR((uint32_t)0);
    SystemInitIrqTable();
    
    GIC_SetPriority(UART1_IRQn, 25);
    DbgConsole_Init(UART1_BASE, 115200, DEBUG_CONSOLE_DEVICE_TYPE_IUART, QEMU_CLK);

    PRINTF("hello 6ul freertos\n");

    while (tasks[i].entry != NULL) {
        if (xTaskCreate(tasks[i].entry, 
                        tasks[i].name, 
                        tasks[i].stack_size,  
                        NULL, 
                        tasks[i].prio, 
                        NULL) != pdPASS) {
            PRINTF("%s creation failed!.\n", tasks[i].name);
            while (1);
        } else {
            PRINTF("%s creation success!.\n", tasks[i].name);
        }
        i++;
    }
 
    vTaskStartScheduler();
    for (;;);
}

