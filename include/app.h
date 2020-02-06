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
#define ECHO_BUF_SIZE 32

extern void uart_poll_demo(void); 
extern void uart_task(void *);
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)

extern void epit1_task(void *);

extern void hello_task2(void *);
extern void hello_task1(void *);

/* Demo debug console */
extern void print_task(void *);
extern void control_task(void *);
extern void console_task(void *);
extern TaskHandle_t g_print_task_handle;
extern TaskHandle_t g_control_task_handle;
extern TaskHandle_t g_console_task_handle;

#endif