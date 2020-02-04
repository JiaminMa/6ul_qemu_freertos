#ifndef _APP_H_
#define _APP_H_

#include <stdint.h>
#define QEMU_CLK    1

extern void uart_poll_demo(void); 
extern void uart_task(void *);
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)

extern void epit1_task(void *);

extern void hello_task2(void *);
extern void hello_task1(void *);

extern void uart1_task(void *);
extern void uart1_init(void);

#endif