#include <app.h>
#include <print.h>

extern void setup_trace(void);

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

#if 0
    {hello_task1, "hello_task1", configMAX_PRIORITIES - 1, 128, NULL},
    {hello_task2, "hello_task2", configMAX_PRIORITIES - 1, 128, NULL},
    {
        epit1_task,
        "epit1_task",
        configMAX_PRIORITIES - 1,
        128,
    },
    
    { control_task, "control_task", configMAX_PRIORITIES - 1, 
        128, &g_control_task_handle},
#endif  
    { print_task, "print_task", configMAX_PRIORITIES - 1,
        128, &g_print_task_handle},
    {NULL, NULL, 0, 0, NULL}
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
    
    print_init();
    trace("hello 6ul freertos\n");

    setup_trace();

    while (tasks[i].entry != NULL) {
        if (xTaskCreate(tasks[i].entry, 
                        tasks[i].name, 
                        tasks[i].stack_size,  
                        NULL, 
                        tasks[i].prio, 
                        tasks[i].task_handle_p) != pdPASS) {
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

