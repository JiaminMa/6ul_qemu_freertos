#ifndef _APP_H_
#define _APP_H_

#include <stdarg.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <stdbool.h>
#include <semphr.h>
#include <system.h>
#include <core_ca7.h>
#include <task.h>
#include <fsl_debug_console.h>

#define QEMU_CLK    1

extern void uart_poll_demo(void); 
extern void uart_task(void *);
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)

extern void epit1_task(void *);

extern void hello_task2(void *);
extern void hello_task1(void *);
extern void print_task(void *);
extern void scanf_task(void *);
extern TaskHandle_t scanfTaskHandle;

#endif