#ifndef _APP_H_
#define _APP_H_

#include <stdint.h>
#define QEMU_CLK    1

extern void uart_poll_demo(void); 
extern void uart_task(void *);

#define DEMO_UART UART1
#define DEMO_UART_CLK_FREQ QEMU_CLK
#define DEMO_IRQn UART1_IRQn
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)

extern void epit1_task(void *);

extern void hello_task2(void *);
extern void hello_task1(void *);
#endif